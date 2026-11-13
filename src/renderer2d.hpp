#pragma once

#include "camera.hpp"
#include "texture.hpp"
#include "transform.hpp"

#include <glm/glm.hpp>

#include <string>

class Shader;

class Renderer2D
{
public:
    Renderer2D() = default;
    ~Renderer2D();

    Renderer2D(const Renderer2D&)            = delete;
    Renderer2D& operator=(const Renderer2D&) = delete;
    Renderer2D(Renderer2D&&)                 = delete;
    Renderer2D& operator=(Renderer2D&&)      = delete;

    bool Init(const std::string& shaderDirectory);
    void Shutdown();

    void SetViewport(int width, int height);
    void SetClearColor(float red, float green, float blue, float alpha);

    void Begin(const Camera& camera);

    void DrawQuad(const Texture& texture, const glm::mat4& model, const glm::vec4& color);
    void DrawQuad(const Texture& texture, const Transform& transform);
    void DrawQuad(const Texture& texture, const Transform& transform, const glm::vec4& color);

    void End();

    Shader* GetShader() const;

private:
    bool CreateQuadGeometry();

    Shader* m_shader = nullptr;

    unsigned int m_vertexArray  = 0;
    unsigned int m_vertexBuffer = 0;
    unsigned int m_indexBuffer  = 0;

    float m_clearRed   = 0.10f;
    float m_clearGreen = 0.11f;
    float m_clearBlue  = 0.15f;
    float m_clearAlpha = 1.0f;

    static constexpr int kQuadIndexCount = 6;
};
