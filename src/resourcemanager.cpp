#include "resourcemanager.hpp"

#include <iostream>

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

Texture* ResourceManager::LoadTexture(const std::string& identifier)
{
    const auto existing = m_textures.find(identifier);
    if (existing != m_textures.end())
    {
        return existing->second.get();
    }

    std::unique_ptr<Texture> texture = std::make_unique<Texture>();

    if (!texture->LoadFromFile(identifier))
    {
        std::cerr << "ResourceManager: failed to load texture '"
                  << identifier << "'" << std::endl;
        return nullptr;
    }

    Texture* rawPointer = texture.get();

    m_textures.emplace(identifier, std::move(texture));

    return rawPointer;
}

Texture* ResourceManager::GetTexture(const std::string& identifier) const
{
    const auto existing = m_textures.find(identifier);

    if (existing == m_textures.end())
    {
        return nullptr;
    }

    return existing->second.get();
}

Texture* ResourceManager::GetWhiteTexture()
{
    if (m_whiteTexture != nullptr)
    {
        return m_whiteTexture.get();
    }

    std::unique_ptr<Texture> texture = std::make_unique<Texture>();

    if (!texture->CreateWhite())
    {
        std::cerr << "ResourceManager: failed to create default white texture" << std::endl;
        return nullptr;
    }

    m_whiteTexture = std::move(texture);

    return m_whiteTexture.get();
}

Shader* ResourceManager::LoadShader(const std::string& identifier,
                                    const std::string& vertexPath,
                                    const std::string& fragmentPath)
{
    const auto existing = m_shaders.find(identifier);
    if (existing != m_shaders.end())
    {
        return existing->second.get();
    }

    std::unique_ptr<Shader> shader = std::make_unique<Shader>();

    if (!shader->LoadFromFiles(vertexPath, fragmentPath))
    {
        std::cerr << "ResourceManager: failed to load shader '"
                  << identifier << "'" << std::endl;
        return nullptr;
    }

    Shader* rawPointer = shader.get();

    m_shaders.emplace(identifier, std::move(shader));

    return rawPointer;
}

Shader* ResourceManager::GetShader(const std::string& identifier) const
{
    const auto existing = m_shaders.find(identifier);

    if (existing == m_shaders.end())
    {
        return nullptr;
    }

    return existing->second.get();
}

bool ResourceManager::HasTexture(const std::string& identifier) const
{
    return m_textures.find(identifier) != m_textures.end();
}

bool ResourceManager::HasShader(const std::string& identifier) const
{
    return m_shaders.find(identifier) != m_shaders.end();
}

std::size_t ResourceManager::GetTextureCount() const
{
    return m_textures.size();
}

std::size_t ResourceManager::GetShaderCount() const
{
    return m_shaders.size();
}

void ResourceManager::Clear()
{
    m_textures.clear();
    m_shaders.clear();
    m_whiteTexture.reset();

    std::cout << "ResourceManager cleared" << std::endl;
}
