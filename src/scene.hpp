#pragma once

#include "camera.hpp"
#include "gameobject.hpp"

#include <memory>
#include <string>
#include <vector>

class Renderer2D;
class SpriteRenderer;

class Scene
{
public:
    Scene() = default;
    explicit Scene(std::string name);
    ~Scene() = default;

    Scene(const Scene&)            = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&)                 = delete;
    Scene& operator=(Scene&&)      = delete;

    GameObject* CreateGameObject(const std::string& name);
    GameObject* AddGameObject(std::unique_ptr<GameObject> gameObject);

    bool RemoveGameObject(GameObject* gameObject);
    bool RemoveGameObject(const std::string& name);

    GameObject* FindGameObject(const std::string& name) const;

    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;
    std::size_t GetGameObjectCount() const;

    Camera& GetCamera();
    const Camera& GetCamera() const;

    void SetViewportSize(int width, int height);

    void Update(float deltaTime);
    void Render(Renderer2D& renderer);

    void Clear();

    const std::string& GetName() const;

private:
    void CollectSprites();

    std::string m_name = "Scene";

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::vector<SpriteRenderer*> m_renderQueue;

    Camera m_camera;
};
