#include "spriterenderer.hpp"

#include "gameobject.hpp"
#include "renderer2d.hpp"
#include "resourcemanager.hpp"
#include "texture.hpp"

#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(GameObject* owner)
    : IComponent(owner)
{
}

void SpriteRenderer::SetTexture(Texture* texture)
{
    m_texture = texture;
}

void SpriteRenderer::SetTexture(const std::string& identifier)
{
    m_texture = ResourceManager::Instance().LoadTexture(identifier);
}

void SpriteRenderer::ClearTexture()
{
    m_texture = nullptr;
}

Texture* SpriteRenderer::GetTexture() const
{
    return m_texture;
}

void SpriteRenderer::SetColor(const glm::vec4& color)
{
    m_color = color;
}

void SpriteRenderer::SetColor(float red, float green, float blue, float alpha)
{
    m_color = glm::vec4(red, green, blue, alpha);
}

const glm::vec4& SpriteRenderer::GetColor() const
{
    return m_color;
}

void SpriteRenderer::SetSize(const glm::vec2& size)
{
    m_size = size;
}

void SpriteRenderer::SetSize(float width, float height)
{
    m_size = glm::vec2(width, height);
}

const glm::vec2& SpriteRenderer::GetSize() const
{
    return m_size;
}

void SpriteRenderer::SetLayer(int layer)
{
    m_layer = layer;
}

int SpriteRenderer::GetLayer() const
{
    return m_layer;
}

void SpriteRenderer::SetVisible(bool visible)
{
    m_visible = visible;
}

bool SpriteRenderer::IsVisible() const
{
    return m_visible;
}

void SpriteRenderer::Render(Renderer2D& renderer) const
{
    if (!m_visible)
    {
        return;
    }

    const GameObject* owner = GetOwner();
    if (owner == nullptr)
    {
        return;
    }

    const Texture* texture = m_texture;

    if (texture == nullptr)
    {
        texture = ResourceManager::Instance().GetWhiteTexture();
    }

    if (texture == nullptr)
    {
        return;
    }

    glm::mat4 model = owner->GetTransform().GetModelMatrix();

    model = glm::scale(model, glm::vec3(m_size, 1.0f));

    renderer.DrawQuad(*texture, model, m_color);
}
