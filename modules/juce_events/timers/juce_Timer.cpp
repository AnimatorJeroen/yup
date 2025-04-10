/*
  ==============================================================================

   This file is part of the YUP library.
   Copyright (c) 2024 - kunitoki@gmail.com

   YUP is an open source library subject to open-source licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   YUP IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2022 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

class Timer::TimerThread final : private Thread
    , private DeletedAtShutdown
{
    static inline constexpr int maxTimeoutMilliseconds = 10;

public:
    using LockType = CriticalSection; // (mysteriously, using a SpinLock here causes problems on some XP machines..)

    JUCE_DECLARE_SINGLETON (TimerThread, true)

    TimerThread()
        : Thread ("JUCE Timer")
    {
        timers.reserve (32);
    }

    ~TimerThread() override
    {
        signalThreadShouldExit();
        callbackArrived.signal();
        stopThread (-1);

        clearSingletonInstance();
    }

    void run() override
    {
        auto lastTime = Time::getMillisecondCounter();
        ReferenceCountedObjectPtr<CallTimersMessage> messageToSend (new CallTimersMessage());

        while (! threadShouldExit())
        {
            auto now = Time::getMillisecondCounter();
            auto elapsed = (int) (now >= lastTime ? (now - lastTime)
                                                  : (std::numeric_limits<uint32>::max() - (lastTime - now)));
            lastTime = now;

            auto timeUntilFirstTimer = getTimeUntilFirstTimer (elapsed);

            if (timeUntilFirstTimer <= 0)
            {
                if (callbackArrived.wait (0))
                {
                    // already a message in flight - do nothing..
                }
                else
                {
                    messageToSend->post();

                    if (! callbackArrived.wait (300))
                    {
                        // Sometimes our message can get discarded by the OS (e.g. when running as an RTAS
                        // when the app has a modal loop), so this is how long to wait before assuming the
                        // message has been lost and trying again.
                        messageToSend->post();
                    }

                    continue;
                }
            }

            // don't wait for too long because running this loop also helps keep the
            // Time::getApproximateMillisecondTimer value stay up-to-date
            wait (jlimit (1, maxTimeoutMilliseconds, timeUntilFirstTimer));
        }
    }

    void callTimers()
    {
        auto now = Time::getMillisecondCounter();
        auto timeout = now + maxTimeoutMilliseconds;

#if JUCE_EMSCRIPTEN && !defined(__EMSCRIPTEN_PTHREADS__)
        auto elapsed = (int) (now >= lastCallTime ? (now - lastCallTime)
                                                  : (std::numeric_limits<uint32>::max() - (lastCallTime - now)));
        lastCallTime = now;

        const LockType::ScopedLockType sl (lock);

        countdownTimers (elapsed);

#else
        const LockType::ScopedLockType sl (lock);

#endif

        while (! timers.empty())
        {
            auto& first = timers.front();

            if (first.countdownMs > 0)
                break;

            auto* timer = first.timer;
            first.countdownMs = timer->timerPeriodMs;
            shuffleTimerBackInQueue (0);
            notify();

            const LockType::ScopedUnlockType ul (lock);

            JUCE_TRY
            {
                timer->timerCallback();
            }
            JUCE_CATCH_EXCEPTION

            // avoid getting stuck in a loop if a timer callback repeatedly takes too long
            if (Time::getMillisecondCounter() > timeout)
                break;
        }

        callbackArrived.signal();
    }

    void callTimersSynchronously()
    {
        callTimers();
    }

    void addTimer (Timer* t)
    {
        const LockType::ScopedLockType sl (lock);

        if (! isThreadRunning())
            startThread (Thread::Priority::high);

        // Trying to add a timer that's already here - shouldn't get to this point,
        // so if you get this assertion, let me know!
        jassert (std::none_of (timers.begin(), timers.end(), [t] (TimerCountdown i)
                               {
                                   return i.timer == t;
                               }));

        auto pos = timers.size();

        timers.push_back ({ t, t->timerPeriodMs });
        t->positionInQueue = pos;
        shuffleTimerForwardInQueue (pos);
        notify();
    }

    void removeTimer (Timer* t)
    {
        const LockType::ScopedLockType sl (lock);

        auto pos = t->positionInQueue;
        auto lastIndex = timers.size() - 1;

        jassert (pos <= lastIndex);
        jassert (timers[pos].timer == t);

        for (auto i = pos; i < lastIndex; ++i)
        {
            timers[i] = timers[i + 1];
            timers[i].timer->positionInQueue = i;
        }

        timers.pop_back();
    }

    void resetTimerCounter (Timer* t) noexcept
    {
        const LockType::ScopedLockType sl (lock);

        auto pos = t->positionInQueue;

        jassert (pos < timers.size());
        jassert (timers[pos].timer == t);

        auto lastCountdown = timers[pos].countdownMs;
        auto newCountdown = t->timerPeriodMs;

        if (newCountdown != lastCountdown)
        {
            timers[pos].countdownMs = newCountdown;

            if (newCountdown > lastCountdown)
                shuffleTimerBackInQueue (pos);
            else
                shuffleTimerForwardInQueue (pos);

            notify();
        }
    }

private:
    LockType lock;

    struct TimerCountdown
    {
        Timer* timer;
        int countdownMs;
    };

    std::vector<TimerCountdown> timers;
    uint32 lastCallTime = 0;

    WaitableEvent callbackArrived;

    struct CallTimersMessage final : public MessageManager::MessageBase
    {
        CallTimersMessage() = default;

        void messageCallback() override
        {
            if (auto* instance = TimerThread::getInstanceWithoutCreating())
                instance->callTimers();
        }
    };

    //==============================================================================
    void shuffleTimerBackInQueue (size_t pos)
    {
        auto numTimers = timers.size();

        if (pos < numTimers - 1)
        {
            auto t = timers[pos];

            for (;;)
            {
                auto next = pos + 1;

                if (next == numTimers || timers[next].countdownMs >= t.countdownMs)
                    break;

                timers[pos] = timers[next];
                timers[pos].timer->positionInQueue = pos;

                ++pos;
            }

            timers[pos] = t;
            t.timer->positionInQueue = pos;
        }
    }

    void shuffleTimerForwardInQueue (size_t pos)
    {
        if (pos > 0)
        {
            auto t = timers[pos];

            while (pos > 0)
            {
                auto& prev = timers[(size_t) pos - 1];

                if (prev.countdownMs <= t.countdownMs)
                    break;

                timers[pos] = prev;
                timers[pos].timer->positionInQueue = pos;

                --pos;
            }

            timers[pos] = t;
            t.timer->positionInQueue = pos;
        }
    }

    int getTimeUntilFirstTimer (int numMillisecsElapsed)
    {
        const LockType::ScopedLockType sl (lock);

        if (timers.empty())
            return 1000;

        countdownTimers (numMillisecsElapsed);

        return timers.front().countdownMs;
    }

    void countdownTimers (int numMillisecsElapsed)
    {
        for (auto& t : timers)
            t.countdownMs -= numMillisecsElapsed;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimerThread)
};

JUCE_IMPLEMENT_SINGLETON (Timer::TimerThread)

//==============================================================================
Timer::Timer() noexcept {}

Timer::Timer (const Timer&) noexcept {}

Timer::~Timer()
{
    // If you're destroying a timer on a background thread, make sure the timer has
    // been stopped before execution reaches this point. A simple way to achieve this
    // is to add a call to `stopTimer()` to the destructor of your class which inherits
    // from Timer.
    jassert (! isTimerRunning()
             || MessageManager::getInstanceWithoutCreating() == nullptr
             || MessageManager::getInstanceWithoutCreating()->currentThreadHasLockedMessageManager());

    stopTimer();
}

void Timer::startTimer (int interval) noexcept
{
    // If you're calling this before (or after) the MessageManager is
    // running, then you're not going to get any timer callbacks!
    JUCE_ASSERT_MESSAGE_MANAGER_EXISTS

    if (auto* instance = TimerThread::getInstance())
    {
        bool wasStopped = (timerPeriodMs == 0);
        timerPeriodMs = jmax (1, interval);

        if (wasStopped)
            instance->addTimer (this);
        else
            instance->resetTimerCounter (this);
    }
}

void Timer::startTimerHz (int timerFrequencyHz) noexcept
{
    if (timerFrequencyHz > 0)
        startTimer (1000 / timerFrequencyHz);
    else
        stopTimer();
}

void Timer::stopTimer() noexcept
{
    if (timerPeriodMs > 0)
    {
        if (auto* instance = TimerThread::getInstanceWithoutCreating())
            instance->removeTimer (this);

        timerPeriodMs = 0;
    }
}

void JUCE_CALLTYPE Timer::callPendingTimersSynchronously()
{
    if (auto* instance = TimerThread::getInstanceWithoutCreating())
        instance->callTimersSynchronously();
}

struct LambdaInvoker final : private Timer
    , private DeletedAtShutdown
{
    LambdaInvoker (int milliseconds, std::function<void()> f)
        : function (std::move (f))
    {
        startTimer (milliseconds);
    }

    ~LambdaInvoker() final
    {
        stopTimer();
    }

    void timerCallback() final
    {
        NullCheckedInvocation::invoke (function);
        delete this;
    }

    std::function<void()> function;

    JUCE_DECLARE_NON_COPYABLE (LambdaInvoker)
};

void JUCE_CALLTYPE Timer::callAfterDelay (int milliseconds, std::function<void()> f)
{
    new LambdaInvoker (milliseconds, std::move (f));
}

} // namespace juce
