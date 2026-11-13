#include "testcomponent.hpp"
#include "gameobject.hpp"

#include <cmath>
#include <iostream>

TestComponent::TestComponent(GameObject* owner, float reportInterval)
    : IComponent(owner)
    , m_reportInterval(reportInterval > 0.0f ? reportInterval : 1.0f)
{
}

void TestComponent::Update(float deltaTime)
{
    m_elapsedTime += deltaTime;
    m_reportTimer += deltaTime;
    ++m_updateCount;

    if (m_reportTimer < m_reportInterval)
    {
        return;
    }

    m_reportTimer = 0.0f;

    const GameObject* owner = GetOwner();
    const std::string ownerName = owner != nullptr ? owner->GetName() : "unknown";

    std::cout << "TestComponent on '" << ownerName << "': "
              << m_updateCount << " updates, "
              << m_elapsedTime << " s" << std::endl;
}

int TestComponent::GetUpdateCount() const
{
    return m_updateCount;
}

float TestComponent::GetElapsedTime() const
{
    return m_elapsedTime;
}

CounterComponent::CounterComponent(GameObject* owner)
    : IComponent(owner)
{
}

void CounterComponent::Update(float deltaTime)
{
    (void)deltaTime;
    ++m_count;
}

int CounterComponent::GetCount() const
{
    return m_count;
}

void CounterComponent::Reset()
{
    m_count = 0;
}

SpinComponent::SpinComponent(GameObject* owner, float degreesPerSecond)
    : IComponent(owner)
    , m_speed(degreesPerSecond)
{
}

void SpinComponent::Update(float deltaTime)
{
    GameObject* owner = GetOwner();

    if (owner == nullptr)
    {
        return;
    }

    owner->GetTransform().Rotate(m_speed * deltaTime);
}

void SpinComponent::SetSpeed(float degreesPerSecond)
{
    m_speed = degreesPerSecond;
}

float SpinComponent::GetSpeed() const
{
    return m_speed;
}

PulseComponent::PulseComponent(GameObject* owner, float baseScale,
                               float amplitude, float frequency)
    : IComponent(owner)
    , m_baseScale(baseScale)
    , m_amplitude(amplitude)
    , m_frequency(frequency)
{
}

void PulseComponent::Update(float deltaTime)
{
    GameObject* owner = GetOwner();

    if (owner == nullptr)
    {
        return;
    }

    m_elapsed += deltaTime;

    const float scale = m_baseScale + m_amplitude * std::sin(m_elapsed * m_frequency);

    owner->GetTransform().SetScale(scale);
}
