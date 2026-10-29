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

GLint Shader::GetUniformLocation(const std::string& name)
{
    const auto cached = m_uniformLocations.find(name);
    if (cached != m_uniformLocations.end())
    {
        return cached->second;
    }

    const GLint location = glGetUniformLocation(m_program, name.c_str());

    if (location == -1)
    {
        std::cerr << "Shader: uniform '" << name << "' not found or unused" << std::endl;
    }

    m_uniformLocations[name] = location;
    return location;
}

void Shader::SetInt(const std::string& name, int value)
{
    const GLint location = GetUniformLocation(name);
    if (location == -1)
    {
        return;
    }

    Bind();
    glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& name, float value)
{
    const GLint location = GetUniformLocation(name);
    if (location == -1)
    {
        return;
    }

    Bind();
    glUniform1f(location, value);
}

void Shader::SetMat4(const std::string& name, const float* values)
{
    const GLint location = GetUniformLocation(name);
    if (location == -1)
    {
        return;
    }

    Bind();
    glUniformMatrix4fv(location, 1, GL_FALSE, values);
}

void Shader::Release()
{
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }

    m_uniformLocations.clear();
}

bool Shader::IsValid() const
{
    return m_program != 0;
}
