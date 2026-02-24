#pragma once

#include <glm/mat4x4.hpp>

namespace Core
{

class OrthographicCamera
{
public:
	OrthographicCamera(float left, float right, float bottom, float top);

	void SetProjection(float left, float right, float bottom, float top);

	const glm::mat4& viewMatrix() const { return m_viewMatrix; }
	const glm::mat4& projectionMatrix() const { return m_projectionMatrix; }
	glm::mat4 viewProjectionMatrix() const { return m_projectionMatrix * m_viewMatrix; }

	void SetPosition(const glm::vec3& position);
	const glm::vec3& GetPosition() const { return m_position; }

	void SetRotation(float rotation);
	float GetRotation() const { return m_rotation; }

private:

	void RecalculateViewMatrix();

private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	glm::vec3 m_position; // Position of the camera
	float m_rotation; // Rotation around the z-axis
};

}
