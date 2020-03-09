
#ifndef FRAME_HPP
#define FRAME_HPP

#include <time.h>

struct Frame
{
private:
    size_t count;
    timespec tp;
    long delta;
public:
    explicit Frame() noexcept
        : count {0}
        , tp {}
        , delta {0}
    {
        clock_gettime(CLOCK_REALTIME_COARSE, &tp);
    }
    
    void Update() noexcept
    {
        ++count;

        long prev_nsec = tp.tv_nsec;
        clock_gettime(CLOCK_REALTIME_COARSE, &tp);
        delta = tp.tv_nsec - prev_nsec;
        delta += (delta < 0) ? 1e9 : 0;
    }

    const size_t& Count() const noexcept
    {
        return count;
    }
    const long& Time() const noexcept
    {
        return tp.tv_sec;
    }
    const long& NanoTime() const noexcept
    {
        return tp.tv_nsec;
    }
    const long& DeltaTime() const  noexcept
    {
        return delta;
    }
};


#ifdef TEST

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int frame_test()
{
    Frame frame;

    printf("Time        : %lus\n", frame.Time());
    printf("Nano time   : %luns\n", frame.NanoTime());
    printf("\n");
    
    auto heavy = []
    {
        size_t sum = 0;
        for (size_t i = 0; i < 1e8; ++i)
        {
            sum += i;
        }
    };

    for (size_t i = 0; i < 8; ++i)
    {
        frame.Update();
        printf("Frame count : %lu\n", frame.Count());
        printf("Delta time  : %ldns\n", frame.DeltaTime());

        heavy();
        printf("Heavy\n");
        printf("\n");
    }

    return 0;
}

#endif

#endif
