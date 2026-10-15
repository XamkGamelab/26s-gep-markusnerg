#pragma once

#include "shader.hpp"

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
    void DrawQuad();
    void End();

private:
    bool CreateQuadGeometry();

    Shader m_shader;

    unsigned int m_vertexArray  = 0;
    unsigned int m_vertexBuffer = 0;
    unsigned int m_indexBuffer  = 0;

    float m_clearRed   = 0.12f;
    float m_clearGreen = 0.12f;
    float m_clearBlue  = 0.16f;
    float m_clearAlpha = 1.0f;

    static constexpr int kQuadIndexCount = 6;
};
