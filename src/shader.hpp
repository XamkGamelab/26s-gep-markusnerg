#pragma once

#include <glad/gl.h>

#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&)                 = delete;
    Shader& operator=(Shader&&)      = delete;

    bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetMat4(const std::string& name, const float* values);

    void Release();

    bool IsValid() const;

private:
    static GLuint CompileStage(GLenum stage, const std::string& source, const std::string& path);
    static bool CheckCompileStatus(GLuint shader, const std::string& path);
    static bool CheckLinkStatus(GLuint program);

    GLint GetUniformLocation(const std::string& name);

    GLuint m_program = 0;

    std::unordered_map<std::string, GLint> m_uniformLocations;
};
