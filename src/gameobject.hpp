#pragma once

#include "icomponent.hpp"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

class GameObject
{
public:
    GameObject() = default;
    explicit GameObject(std::string name);
    ~GameObject() = default;

    GameObject(const GameObject&)            = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&)                 = delete;
    GameObject& operator=(GameObject&&)      = delete;

    template <typename TComponent, typename... TArgs>
    TComponent* AddComponent(TArgs&&... args)
    {
        static_assert(std::is_base_of<IComponent, TComponent>::value,
            "TComponent must inherit from IComponent");

        std::unique_ptr<TComponent> component =
            std::make_unique<TComponent>(this, std::forward<TArgs>(args)...);

        TComponent* rawPointer = component.get();

        m_components.push_back(std::move(component));

        return rawPointer;
    }

    template <typename TComponent>
    TComponent* GetComponent() const
    {
        static_assert(std::is_base_of<IComponent, TComponent>::value,
            "TComponent must inherit from IComponent");

        for (const std::unique_ptr<IComponent>& component : m_components)
        {
            TComponent* typed = dynamic_cast<TComponent*>(component.get());

            if (typed != nullptr)
            {
                return typed;
            }
        }

        return nullptr;
    }

    void Update(float deltaTime);

    const std::string& GetName() const;
    void SetName(std::string name);

    std::size_t GetComponentCount() const;

private:
    std::string m_name = "GameObject";

    std::vector<std::unique_ptr<IComponent>> m_components;
};
