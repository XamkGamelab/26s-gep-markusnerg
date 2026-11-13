#pragma once

#include "shader.hpp"
#include "texture.hpp"

#include <memory>
#include <string>
#include <unordered_map>

class ResourceManager
{
public:
    static ResourceManager& Instance();

    ResourceManager(const ResourceManager&)            = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&)                 = delete;
    ResourceManager& operator=(ResourceManager&&)      = delete;

    Texture* LoadTexture(const std::string& identifier);
    Texture* GetTexture(const std::string& identifier) const;
    Texture* GetWhiteTexture();

    Shader* LoadShader(const std::string& identifier,
                       const std::string& vertexPath,
                       const std::string& fragmentPath);
    Shader* GetShader(const std::string& identifier) const;

    bool HasTexture(const std::string& identifier) const;
    bool HasShader(const std::string& identifier) const;

    std::size_t GetTextureCount() const;
    std::size_t GetShaderCount() const;

    void Clear();

private:
    ResourceManager()  = default;
    ~ResourceManager() = default;

    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
    std::unordered_map<std::string, std::unique_ptr<Shader>>  m_shaders;

    std::unique_ptr<Texture> m_whiteTexture;
};
