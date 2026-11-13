#pragma once

#include "icomponent.hpp"

#include <glm/glm.hpp>

#include <string>

class Renderer2D;
class Texture;

class SpriteRenderer : public IComponent
{
public:
    explicit SpriteRenderer(GameObject* owner);

    void SetTexture(Texture* texture);
    void SetTexture(const std::string& identifier);
    void ClearTexture();
    Texture* GetTexture() const;

    void SetColor(const glm::vec4& color);
    void SetColor(float red, float green, float blue, float alpha = 1.0f);
    const glm::vec4& GetColor() const;

    void SetSize(const glm::vec2& size);
    void SetSize(float width, float height);
    const glm::vec2& GetSize() const;

    void SetLayer(int layer);
    int GetLayer() const;

    void SetVisible(bool visible);
    bool IsVisible() const;

    void Render(Renderer2D& renderer) const;

private:
    Texture* m_texture = nullptr;

    glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec2 m_size{ 1.0f, 1.0f };

    int  m_layer   = 0;
    bool m_visible = true;
};
