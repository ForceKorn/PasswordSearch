#pragma once

#include <chrono>

class TimeMeasurement
{
public:
    using clock_t = std::chrono::steady_clock;
public:
    TimeMeasurement();

    inline void updateStartPoint() noexcept
    {
	    m_startPoint = clock_t::now();
    }

    template<typename Duration>
    constexpr typename Duration::rep 
        getElapsedTimeCount(const clock_t::time_point& currentTimePoint = clock_t::now()) const noexcept
    {
        return std::chrono::duration_cast<Duration>(currentTimePoint - m_startPoint).count();
    }

    inline static clock_t::time_point getCurrentTimePoint() noexcept
    {
		return clock_t::now();
    }

private:
    clock_t::time_point m_startPoint;
};
