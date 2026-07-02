#pragma once

#include <OrthographicCamera.h>

#include <Events/Event.h>

namespace Core
{
class MouseScrolledEvent;
}

namespace Core
{

class OrthographicCameraController
{
public:
	OrthographicCameraController(float aspectRatio);

	const OrthographicCamera& GetCamera() const { return m_camera; }
	OrthographicCamera& GetCamera() { return m_camera; }

	const glm::mat4& viewMatrix() const { return m_camera.viewMatrix(); }
	const glm::mat4& projectionMatrix() const { return m_camera.projectionMatrix(); }
	glm::mat4 viewProjectionMatrix() const { return m_camera.viewProjectionMatrix(); }

	void Update(float deltaTime);

	void OnEvent(Event& event);

private:

	bool OnMouseScrolledEvent(const MouseScrolledEvent& event);

private:
	float m_aspectRatio;
	float m_zoomLevel;
	OrthographicCamera m_camera;

	/* Speed parameters */
	float m_horizontalSpeed;
	float m_verticalSpeed;
	float m_rotationSpeed = 5.f;

	/* Camera transform */
	glm::vec3 m_position;
	float m_rotation;
};

}
