#include "texture.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>

Texture::~Texture()
{
    Release();
}

bool Texture::LoadFromFile(const std::string& path)
{
    Release();

    SDL_Surface* loaded = IMG_Load(path.c_str());
    if (loaded == nullptr)
    {
        std::cerr << "Texture: IMG_Load failed for " << path
                  << ": " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Surface* converted = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(loaded);

    if (converted == nullptr)
    {
        std::cerr << "Texture: SDL_ConvertSurface failed for " << path
                  << ": " << SDL_GetError() << std::endl;
        return false;
    }

    if (!SDL_FlipSurface(converted, SDL_FLIP_VERTICAL))
    {
        std::cerr << "Texture: SDL_FlipSurface failed for " << path
                  << ": " << SDL_GetError() << std::endl;
        SDL_DestroySurface(converted);
        return false;
    }

    const bool uploaded = Upload(converted->pixels, converted->w, converted->h, converted->pitch);

    SDL_DestroySurface(converted);

    if (!uploaded)
    {
        std::cerr << "Texture: upload failed for " << path << std::endl;
        return false;
    }

    std::cout << "Texture loaded: " << path << " (" << m_width
              << "x" << m_height << ")" << std::endl;

    return true;
}

bool Texture::CreateFromPixels(const unsigned char* pixels, int width, int height)
{
    Release();

    if (pixels == nullptr || width <= 0 || height <= 0)
    {
        std::cerr << "Texture: invalid pixel data" << std::endl;
        return false;
    }

    return Upload(pixels, width, height, width * 4);
}

bool Texture::CreateWhite()
{
    const unsigned char white[4] = { 255, 255, 255, 255 };

    return CreateFromPixels(white, 1, 1);
}

bool Texture::Upload(const void* pixels, int width, int height, int pitch)
{
    glGenTextures(1, &m_handle);
    if (m_handle == 0)
    {
        std::cerr << "Texture: glGenTextures failed" << std::endl;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, pitch / 4);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_width  = width;
    m_height = height;

    return true;
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Release()
{
    if (m_handle != 0)
    {
        glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }

    m_width  = 0;
    m_height = 0;
}

bool Texture::IsValid() const
{
    return m_handle != 0;
}

int Texture::GetWidth() const
{
    return m_width;
}

int Texture::GetHeight() const
{
    return m_height;
}
