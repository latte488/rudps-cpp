
#ifndef FRAME_TIMER_HPP
#define FRAME_TIMER_HPP

#include <frame.hpp>

class FrameTimer
{
private:
    long m_interval;
    long m_timer;

public:

    void Setup(long nano) noexcept
    {
        m_timer = m_interval = (nano <= 0) ? 0 : nano;
    }

    void Update(Frame& frame) noexcept
    {
        m_timer -= frame.DeltaTime();
    }

    void Reset() noexcept
    {
        m_timer = m_interval;
    }

    bool IsExpired() const noexcept
    {
        return (m_timer <= 0);
    }
};


#ifdef TEST

#include <stdlib.h>
#include <stdio.h>

int frame_timer_test()
{
    Frame frame;
    FrameTimer timer;

    timer.Setup(1e9);

    srand(48);

    size_t s = 0;

    while (s <= 8)
    {
        frame.Update();
        timer.Update(frame);

        if (timer.IsExpired())
        {
            timer.Reset();
            printf("%lus\n", s++);
        }
    }

    return 0;
}

#endif

#endif

