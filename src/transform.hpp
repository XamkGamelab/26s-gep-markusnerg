#pragma once

#include <glm/glm.hpp>

class Transform
{
public:
    Transform() = default;
    Transform(const glm::vec3& position, float rotationDegrees, const glm::vec3& scale);

    void SetPosition(const glm::vec3& position);
    void SetPosition(float x, float y);
    const glm::vec3& GetPosition() const;

    void SetRotation(float degrees);
    float GetRotation() const;

    void SetScale(const glm::vec3& scale);
    void SetScale(float x, float y);
    void SetScale(float uniform);
    const glm::vec3& GetScale() const;

    void Translate(const glm::vec3& delta);
    void Rotate(float degrees);

    glm::mat4 GetModelMatrix() const;

private:
    glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
    float     m_rotation = 0.0f;
    glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
};
