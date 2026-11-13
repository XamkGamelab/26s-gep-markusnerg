#include "gameobject.hpp"

GameObject::GameObject(std::string name)
    : m_name(std::move(name))
{
}

void GameObject::Update(float deltaTime)
{
    if (!m_active)
    {
        return;
    }

    for (const std::unique_ptr<IComponent>& component : m_components)
    {
        component->Update(deltaTime);
    }
}

Transform& GameObject::GetTransform()
{
    return m_transform;
}

const Transform& GameObject::GetTransform() const
{
    return m_transform;
}

const std::string& GameObject::GetName() const
{
    return m_name;
}

void GameObject::SetName(std::string name)
{
    m_name = std::move(name);
}

void GameObject::SetActive(bool active)
{
    m_active = active;
}

bool GameObject::IsActive() const
{
    return m_active;
}

std::size_t GameObject::GetComponentCount() const
{
    return m_components.size();
}
