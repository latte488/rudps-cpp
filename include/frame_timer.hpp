
#ifndef FRAME_TIMER_HPP
#define FRAME_TIMER_HPP

#include <frame.hpp>

class FrameTimer
{
private:
    long interval;
    long timer;

public:

    void Setup(long nano) noexcept
    {
        timer = interval = (nano <= 0) ? 0 : nano;
    }

    void Update(Frame& frame) noexcept
    {
        timer -= frame.DeltaTime();
    }

    void Reset() noexcept
    {
        timer = interval;
    }

    bool IsExpired() const noexcept
    {
        return (timer <= 0);
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

