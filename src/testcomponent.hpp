#pragma once

#include "icomponent.hpp"

class TestComponent : public IComponent
{
public:
    explicit TestComponent(GameObject* owner, float reportInterval = 1.0f);

    void Update(float deltaTime) override;

    int GetUpdateCount() const;
    float GetElapsedTime() const;

private:
    float m_elapsedTime    = 0.0f;
    float m_reportTimer    = 0.0f;
    float m_reportInterval = 1.0f;
    int   m_updateCount    = 0;
};

class CounterComponent : public IComponent
{
public:
    explicit CounterComponent(GameObject* owner);

    void Update(float deltaTime) override;

    int GetCount() const;
    void Reset();

private:
    int m_count = 0;
};

class SpinComponent : public IComponent
{
public:
    explicit SpinComponent(GameObject* owner, float degreesPerSecond = 45.0f);

    void Update(float deltaTime) override;

    void SetSpeed(float degreesPerSecond);
    float GetSpeed() const;

private:
    float m_speed = 45.0f;
};

class PulseComponent : public IComponent
{
public:
    explicit PulseComponent(GameObject* owner, float baseScale = 1.0f,
                            float amplitude = 0.25f, float frequency = 2.0f);

    void Update(float deltaTime) override;

private:
    float m_baseScale = 1.0f;
    float m_amplitude = 0.25f;
    float m_frequency = 2.0f;
    float m_elapsed   = 0.0f;
};
