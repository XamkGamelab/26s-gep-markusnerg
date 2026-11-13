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
