#pragma once

#include <chrono>

class Time
{
public:
    static Time& Instance();

    Time(const Time&)            = delete;
    Time& operator=(const Time&) = delete;
    Time(Time&&)                 = delete;
    Time& operator=(Time&&)      = delete;

    void Tick();

    float GetDeltaTime() const;

private:
    Time()  = default;
    ~Time() = default;

    std::chrono::steady_clock::time_point m_previous{};
    bool  m_initialized = false;
    float m_deltaTime   = 0.0f;

    static constexpr float kMaxDeltaTime = 1.0f / 30.0f;
};
