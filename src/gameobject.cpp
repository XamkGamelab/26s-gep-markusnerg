#include "gameobject.hpp"

GameObject::GameObject(std::string name)
    : m_name(std::move(name))
{
}

void GameObject::Update(float deltaTime)
{
    for (const std::unique_ptr<IComponent>& component : m_components)
    {
        component->Update(deltaTime);
    }
}

const std::string& GameObject::GetName() const
{
    return m_name;
}

void GameObject::SetName(std::string name)
{
    m_name = std::move(name);
}

std::size_t GameObject::GetComponentCount() const
{
    return m_components.size();
}
