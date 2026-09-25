#include "time.hpp"

Time& Time::Instance()
{
    static Time instance;
    return instance;
}

void Time::Tick()
{
    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    if (!m_initialized)
    {
        m_previous    = now;
        m_initialized = true;
        m_deltaTime   = 0.0f;
        return;
    }

    const std::chrono::duration<float> elapsed = now - m_previous;
    m_previous = now;

    m_deltaTime = elapsed.count();

    if (m_deltaTime > kMaxDeltaTime)
    {
        m_deltaTime = kMaxDeltaTime;
    }
}

float Time::GetDeltaTime() const
{
    return m_deltaTime;
}
