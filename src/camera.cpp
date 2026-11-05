#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    RecalculateProjection();
}

void Camera::SetViewportSize(int width, int height)
{
    const int safeWidth  = width  > 0 ? width  : 1;
    const int safeHeight = height > 0 ? height : 1;

    m_aspectRatio = static_cast<float>(safeWidth) / static_cast<float>(safeHeight);

    RecalculateProjection();
}

void Camera::SetZoom(float halfHeight)
{
    m_zoom = halfHeight > 0.01f ? halfHeight : 0.01f;

    RecalculateProjection();
}

float Camera::GetZoom() const
{
    return m_zoom;
}

void Camera::RecalculateProjection()
{
    const float halfWidth  = m_aspectRatio * m_zoom;
    const float halfHeight = m_zoom;

    m_projection = glm::ortho(
        -halfWidth,  halfWidth,
        -halfHeight, halfHeight,
        -1.0f, 1.0f);
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

void Camera::Move(const glm::vec3& delta)
{
    m_position += delta;
}

const glm::vec3& Camera::GetPosition() const
{
    return m_position;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::translate(glm::mat4(1.0f), -m_position);
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
    return m_projection;
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
    return m_projection * GetViewMatrix();
}
