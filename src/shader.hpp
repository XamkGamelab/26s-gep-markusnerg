#pragma once

#include <glad/gl.h>

#include <string>

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

    void Release();

    bool IsValid() const;

private:
    static GLuint CompileStage(GLenum stage, const std::string& source, const std::string& path);
    static bool CheckCompileStatus(GLuint shader, const std::string& path);
    static bool CheckLinkStatus(GLuint program);

    GLuint m_program = 0;
};
