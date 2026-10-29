#pragma once

#include "shader.hpp"
#include "texture.hpp"

#include <string>

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

    void Begin();
    void DrawQuad(const Texture& texture);
    void End();

private:
    bool CreateQuadGeometry();
    void BuildTransform(const Texture& texture, float* outMatrix) const;

    Shader m_shader;

    unsigned int m_vertexArray  = 0;
    unsigned int m_vertexBuffer = 0;
    unsigned int m_indexBuffer  = 0;

    int m_viewportWidth  = 1;
    int m_viewportHeight = 1;

    float m_clearRed   = 0.12f;
    float m_clearGreen = 0.12f;
    float m_clearBlue  = 0.16f;
    float m_clearAlpha = 1.0f;

    static constexpr int   kQuadIndexCount = 6;
    static constexpr float kQuadScale      = 0.8f;
};
