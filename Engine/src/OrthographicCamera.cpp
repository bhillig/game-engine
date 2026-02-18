#include <OrthographicCamera.h>

namespace Core
{

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	: m_viewMatrix(1.f), m_projectionMatrix(glm::orthoLH(left, right, bottom, top, -1.f, 1.f)),
	m_position({ 0.f, 0.f, 0.f }), m_rotation(0.f)
{
}

void OrthographicCamera::SetPosition(const glm::vec3& position)
{
	m_position = position;
	RecalculateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation)
{
	m_rotation = rotation;
	RecalculateViewMatrix();
}

void OrthographicCamera::RecalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.f), m_position)
		* glm::rotate(glm::mat4(1.f), m_rotation, glm::vec3({ 0.f, 0.f, 1.f }));

	m_viewMatrix = glm::inverse(transform);
}

}
