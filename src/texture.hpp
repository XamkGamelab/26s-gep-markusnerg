#pragma once

#include <glad/gl.h>

#include <string>

class Texture
{
public:
    Texture() = default;
    ~Texture();

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&)                 = delete;
    Texture& operator=(Texture&&)      = delete;

    bool LoadFromFile(const std::string& path);

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void Release();

    bool IsValid() const;

    int GetWidth() const;
    int GetHeight() const;

private:
    GLuint m_handle = 0;
    int    m_width  = 0;
    int    m_height = 0;
};
