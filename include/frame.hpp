
#ifndef FRAME_HPP
#define FRAME_HPP

#include <time.h>

struct Frame
{
private:
    size_t m_count;
    timespec m_tp;
    long m_delta;
public:
    explicit Frame() noexcept
        : m_count {0}
        , m_tp {}
        , m_delta {0}
    {
        clock_gettime(CLOCK_REALTIME_COARSE, &m_tp);
    }
    
    void Update() noexcept
    {
        ++m_count;

        long prev_nsec = m_tp.tv_nsec;
        clock_gettime(CLOCK_REALTIME_COARSE, &m_tp);
        m_delta = m_tp.tv_nsec - prev_nsec;
        m_delta += (m_delta < 0) ? 1e9 : 0;
    }

    const size_t& Count() const noexcept
    {
        return m_count;
    }

    const long& Time() const noexcept
    {
        return m_tp.tv_sec;
    }

    const long& NanoTime() const noexcept
    {
        return m_tp.tv_nsec;
    }

    const long& DeltaTime() const  noexcept
    {
        return m_delta;
    }
};


#ifdef TEST

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int frame_test()
{
    auto heavy = []
    {
        size_t sum = 0;
        for (size_t i = 0; i < 1e8; ++i)
        {
            sum += i;
        }
    };

    Frame frame;
    
    for (size_t i = 0; i < 8; ++i)
    {
        frame.Update();
        printf("frame.Count     : %lu\n",   frame.Count());
        printf("frame.Time      : %lus\n",  frame.Time());
        printf("frame.NanoTime  : %luns\n", frame.NanoTime());
        printf("frame.DeltaTime : %ldns\n", frame.DeltaTime());

        heavy();
    }

    return 0;
}

#endif

#endif

