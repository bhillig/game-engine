#include <OrthographicCameraController.h>

#include <Events/WindowEvents.h>

#include <Input.h>

#include "Events/InputEvents.h"

namespace Core
{

OrthographicCameraController::OrthographicCameraController(float aspectRatio)
	: m_aspectRatio(aspectRatio), m_zoomLevel(1.f), m_camera(aspectRatio * -m_zoomLevel, aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel)
	, m_horizontalSpeed(1.f), m_verticalSpeed(1.f), m_position({0.f, 0.f, 0.f}), m_rotation(0.f)
{
}

void OrthographicCameraController::Update(float deltaTime)
{
	// Apply movement
	if (Input::IsKeyPressed(KEY_W))
	{
		// Move up
		m_position.y += m_verticalSpeed * deltaTime;
	}
	else if (Input::IsKeyPressed(KEY_S))
	{
		// Move down
		m_position.y += -m_verticalSpeed * deltaTime;
	}

	if (Input::IsKeyPressed(KEY_D))
	{
		// Move right
		m_position.x += m_horizontalSpeed * deltaTime;
	}
	else if (Input::IsKeyPressed(KEY_A))
	{
		// Move left
		m_position.x += -m_horizontalSpeed * deltaTime;
	}

	m_camera.SetPosition(m_position);

	// Apply rotation
	if (Input::IsKeyPressed(KEY_R))
	{
		// Rotate left
		m_rotation += m_rotationSpeed * deltaTime;
	}
	else if (Input::IsKeyPressed(KEY_T))
	{
		// Rotate right
		m_rotation += -m_rotationSpeed * deltaTime;
	}

	m_camera.SetRotation(m_rotation);
}

void OrthographicCameraController::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);

	dispatcher.Dispatch<MouseScrolledEvent>(CORE_BIND_FN(OrthographicCameraController::OnMouseScrolledEvent));
}

bool OrthographicCameraController::OnMouseScrolledEvent(const MouseScrolledEvent& event)
{
	m_zoomLevel -= event.GetYOffset();
	m_zoomLevel = std::max(m_zoomLevel, 0.25f);

	// Recalculate camera projection matrix
	m_camera.SetProjection(m_aspectRatio * -m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
	return false;
}
}
