#pragma once

class GameObject;

class IComponent
{
public:
    explicit IComponent(GameObject* owner);
    virtual ~IComponent() = default;

    IComponent(const IComponent&)            = delete;
    IComponent& operator=(const IComponent&) = delete;
    IComponent(IComponent&&)                 = delete;
    IComponent& operator=(IComponent&&)      = delete;

    virtual void Update(float deltaTime);

    GameObject* GetOwner() const;

protected:
    GameObject* m_owner = nullptr;
};
