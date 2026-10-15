#include "shader.hpp"
#include "file_loader.hpp"

#include <iostream>
#include <vector>

Shader::~Shader()
{
    Release();
}

bool Shader::LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
{
    Release();

    std::string vertexSource;
    std::string fragmentSource;

    if (!file_loader::ReadTextFile(vertexPath, vertexSource))
    {
        return false;
    }

    if (!file_loader::ReadTextFile(fragmentPath, fragmentSource))
    {
        return false;
    }

    const GLuint vertexShader = CompileStage(GL_VERTEX_SHADER, vertexSource, vertexPath);
    if (vertexShader == 0)
    {
        return false;
    }

    const GLuint fragmentShader = CompileStage(GL_FRAGMENT_SHADER, fragmentSource, fragmentPath);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return false;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!CheckLinkStatus(program))
    {
        glDeleteProgram(program);
        return false;
    }

    m_program = program;
    return true;
}

GLuint Shader::CompileStage(GLenum stage, const std::string& source, const std::string& path)
{
    const GLuint shader = glCreateShader(stage);

    const char* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    if (!CheckCompileStatus(shader, path))
    {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::CheckCompileStatus(GLuint shader, const std::string& path)
{
    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_TRUE)
    {
        return true;
    }

    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<char> log(logLength > 1 ? static_cast<size_t>(logLength) : size_t{ 1 });
    glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr, log.data());

    std::cerr << "Shader compilation failed (" << path << "):\n" << log.data() << std::endl;
    return false;
}

bool Shader::CheckLinkStatus(GLuint program)
{
    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_TRUE)
    {
        return true;
    }

    GLint logLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<char> log(logLength > 1 ? static_cast<size_t>(logLength) : size_t{ 1 });
    glGetProgramInfoLog(program, static_cast<GLsizei>(log.size()), nullptr, log.data());

    std::cerr << "Shader program linking failed:\n" << log.data() << std::endl;
    return false;
}

void Shader::Bind() const
{
    glUseProgram(m_program);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::Release()
{
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

bool Shader::IsValid() const
{
    return m_program != 0;
}
