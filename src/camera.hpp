#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    void SetViewportSize(int width, int height);

    void SetZoom(float halfHeight);
    float GetZoom() const;

    void SetPosition(const glm::vec3& position);
    void Move(const glm::vec3& delta);
    const glm::vec3& GetPosition() const;

    glm::mat4 GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;

private:
    void RecalculateProjection();

    glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
    glm::mat4 m_projection{ 1.0f };

    float m_aspectRatio = 1.0f;
    float m_zoom        = 3.0f;
};
