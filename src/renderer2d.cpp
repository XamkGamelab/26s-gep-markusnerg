#include "renderer2d.hpp"

#include <glad/gl.h>

#include <iostream>

namespace
{
    const float kQuadVertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    const unsigned int kQuadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };
}

Renderer2D::~Renderer2D()
{
    Shutdown();
}

bool Renderer2D::Init(const std::string& shaderDirectory)
{
    const std::string vertexPath   = shaderDirectory + "quad.vert.glsl";
    const std::string fragmentPath = shaderDirectory + "quad.frag.glsl";

    if (!m_shader.LoadFromFiles(vertexPath, fragmentPath))
    {
        std::cerr << "Renderer2D: shader creation failed" << std::endl;
        return false;
    }

    if (!CreateQuadGeometry())
    {
        std::cerr << "Renderer2D: quad geometry creation failed" << std::endl;
        m_shader.Release();
        return false;
    }

    std::cout << "Renderer2D initialized" << std::endl;
    return true;
}

bool Renderer2D::CreateQuadGeometry()
{
    glGenVertexArrays(1, &m_vertexArray);
    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);

    if (m_vertexArray == 0 || m_vertexBuffer == 0 || m_indexBuffer == 0)
    {
        return false;
    }

    glBindVertexArray(m_vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kQuadVertices), kQuadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kQuadIndices), kQuadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

void Renderer2D::Shutdown()
{
    if (m_indexBuffer != 0)
    {
        glDeleteBuffers(1, &m_indexBuffer);
        m_indexBuffer = 0;
    }

    if (m_vertexBuffer != 0)
    {
        glDeleteBuffers(1, &m_vertexBuffer);
        m_vertexBuffer = 0;
    }

    if (m_vertexArray != 0)
    {
        glDeleteVertexArrays(1, &m_vertexArray);
        m_vertexArray = 0;
    }

    m_shader.Release();
}

void Renderer2D::SetViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Renderer2D::SetClearColor(float red, float green, float blue, float alpha)
{
    m_clearRed   = red;
    m_clearGreen = green;
    m_clearBlue  = blue;
    m_clearAlpha = alpha;
}

void Renderer2D::Begin()
{
    glClearColor(m_clearRed, m_clearGreen, m_clearBlue, m_clearAlpha);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader.Bind();
}

void Renderer2D::DrawQuad()
{
    glBindVertexArray(m_vertexArray);
    glDrawElements(GL_TRIANGLES, kQuadIndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Renderer2D::End()
{
    m_shader.Unbind();
}
