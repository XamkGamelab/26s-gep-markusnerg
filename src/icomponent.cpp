#include "icomponent.hpp"

IComponent::IComponent(GameObject* owner)
    : m_owner(owner)
{
}

void IComponent::Update(float deltaTime)
{
    (void)deltaTime;
}

GameObject* IComponent::GetOwner() const
{
    return m_owner;
}
