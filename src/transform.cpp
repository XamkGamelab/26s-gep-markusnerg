#include "transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(const glm::vec3& position, float rotationDegrees, const glm::vec3& scale)
    : m_position(position)
    , m_rotation(rotationDegrees)
    , m_scale(scale)
{
}

void Transform::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

void Transform::SetPosition(float x, float y)
{
    m_position = glm::vec3(x, y, m_position.z);
}

const glm::vec3& Transform::GetPosition() const
{
    return m_position;
}

void Transform::SetRotation(float degrees)
{
    m_rotation = degrees;
}

float Transform::GetRotation() const
{
    return m_rotation;
}

void Transform::SetScale(const glm::vec3& scale)
{
    m_scale = scale;
}

void Transform::SetScale(float x, float y)
{
    m_scale = glm::vec3(x, y, 1.0f);
}

void Transform::SetScale(float uniform)
{
    m_scale = glm::vec3(uniform, uniform, 1.0f);
}

const glm::vec3& Transform::GetScale() const
{
    return m_scale;
}

void Transform::Translate(const glm::vec3& delta)
{
    m_position += delta;
}

void Transform::Rotate(float degrees)
{
    m_rotation += degrees;

    if (m_rotation >= 360.0f)
    {
        m_rotation -= 360.0f;
    }
    else if (m_rotation <= -360.0f)
    {
        m_rotation += 360.0f;
    }
}

glm::mat4 Transform::GetModelMatrix() const
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);

    model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, m_scale);

    return model;
}
