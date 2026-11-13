#include "scene.hpp"

#include "renderer2d.hpp"
#include "spriterenderer.hpp"

#include <algorithm>

Scene::Scene(std::string name)
    : m_name(std::move(name))
{
}

GameObject* Scene::CreateGameObject(const std::string& name)
{
    std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>(name);

    GameObject* rawPointer = gameObject.get();

    m_gameObjects.push_back(std::move(gameObject));

    return rawPointer;
}

GameObject* Scene::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
    if (gameObject == nullptr)
    {
        return nullptr;
    }

    GameObject* rawPointer = gameObject.get();

    m_gameObjects.push_back(std::move(gameObject));

    return rawPointer;
}

bool Scene::RemoveGameObject(GameObject* gameObject)
{
    if (gameObject == nullptr)
    {
        return false;
    }

    const auto match = std::find_if(
        m_gameObjects.begin(),
        m_gameObjects.end(),
        [gameObject](const std::unique_ptr<GameObject>& candidate)
        {
            return candidate.get() == gameObject;
        });

    if (match == m_gameObjects.end())
    {
        return false;
    }

    m_gameObjects.erase(match);
    m_renderQueue.clear();

    return true;
}

bool Scene::RemoveGameObject(const std::string& name)
{
    return RemoveGameObject(FindGameObject(name));
}

GameObject* Scene::FindGameObject(const std::string& name) const
{
    for (const std::unique_ptr<GameObject>& gameObject : m_gameObjects)
    {
        if (gameObject->GetName() == name)
        {
            return gameObject.get();
        }
    }

    return nullptr;
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
{
    return m_gameObjects;
}

std::size_t Scene::GetGameObjectCount() const
{
    return m_gameObjects.size();
}

Camera& Scene::GetCamera()
{
    return m_camera;
}

const Camera& Scene::GetCamera() const
{
    return m_camera;
}

void Scene::SetViewportSize(int width, int height)
{
    m_camera.SetViewportSize(width, height);
}

void Scene::Update(float deltaTime)
{
    for (const std::unique_ptr<GameObject>& gameObject : m_gameObjects)
    {
        gameObject->Update(deltaTime);
    }
}

void Scene::CollectSprites()
{
    m_renderQueue.clear();

    for (const std::unique_ptr<GameObject>& gameObject : m_gameObjects)
    {
        if (!gameObject->IsActive())
        {
            continue;
        }

        SpriteRenderer* sprite = gameObject->GetComponent<SpriteRenderer>();

        if (sprite != nullptr)
        {
            m_renderQueue.push_back(sprite);
        }
    }

    std::stable_sort(
        m_renderQueue.begin(),
        m_renderQueue.end(),
        [](const SpriteRenderer* left, const SpriteRenderer* right)
        {
            return left->GetLayer() < right->GetLayer();
        });
}

void Scene::Render(Renderer2D& renderer)
{
    CollectSprites();

    renderer.Begin(m_camera);

    for (SpriteRenderer* sprite : m_renderQueue)
    {
        sprite->Render(renderer);
    }

    renderer.End();
}

void Scene::Clear()
{
    m_renderQueue.clear();
    m_gameObjects.clear();
}

const std::string& Scene::GetName() const
{
    return m_name;
}
