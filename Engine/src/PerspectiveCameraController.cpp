#include <PerspectiveCameraController.h>

#include <glm/glm.hpp>

namespace Core
{

PerspectiveCameraController::PerspectiveCameraController(PerspectiveCamera& camera)
	: m_camera(camera)
	, m_mouseHorizontalSensitivity(0.1f)
	, m_mouseVerticalSensitivity(0.1f)
	, m_forwardPressed(false)
	, m_backwardPressed(false)
	, m_leftPressed(false)
	, m_rightPressed(false)
	, m_firstMouseMoveEvent(true)
{
}

PerspectiveCameraController::~PerspectiveCameraController()
{
}

void PerspectiveCameraController::Update(float deltaTime)
{
	const glm::vec3 cameraForward = m_camera.forward();
	const float movementSpeed = 15.f;
	const glm::vec3 upVector = glm::vec3(0.f, 1.0f, 0.f);

	glm::vec3 moveDelta(0.0f);
	if (m_forwardPressed)
	{
		moveDelta = movementSpeed * cameraForward * deltaTime;
	}
	if (m_backwardPressed)
	{
		moveDelta = -(movementSpeed * cameraForward * deltaTime);
	}
	if (m_leftPressed)
	{
		moveDelta = -(glm::normalize(glm::cross(upVector, cameraForward)) * movementSpeed * deltaTime);
	}
	if (m_rightPressed)
	{
		moveDelta = glm::normalize(glm::cross(upVector, cameraForward)) * movementSpeed * deltaTime;
	}

	m_camera.Move(moveDelta);
}

void PerspectiveCameraController::OnMouseMove(double xPos, double yPos)
{
	static double lastX;
	static double lastY;

	if (m_firstMouseMoveEvent)
	{
		lastX = xPos;
		lastY = yPos;
		m_firstMouseMoveEvent = false;
	}

	const float deltaX = xPos - lastX;
	const float deltaY = yPos - lastY;

	const float yawOffset = -deltaX * m_mouseHorizontalSensitivity;
	const float pitchOffset = -deltaY * m_mouseVerticalSensitivity;

	m_camera.Rotate(pitchOffset, yawOffset, 0.f);

	lastX = xPos;
	lastY = yPos;
}

void PerspectiveCameraController::OnKeyPressed(int key)
{
	if (key == KEY_W)
	{
		m_forwardPressed = true;
	}
	else if (key == KEY_S)
	{
		m_backwardPressed = true;
	}
	else if (key == KEY_A)
	{
		m_leftPressed = true;
	}
	else if (key == KEY_D)
	{
		m_rightPressed = true;
	}
}

void PerspectiveCameraController::OnKeyReleased(int key)
{
	if (key == KEY_W)
	{
		m_forwardPressed = false;
	}
	else if (key == KEY_S)
	{
		m_backwardPressed = false;
	}
	else if (key == KEY_A)
	{
		m_leftPressed = false;
	}
	else if (key == KEY_D)
	{
		m_rightPressed = false;
	}
}

void PerspectiveCameraController::OnLoseControl()
{
	m_firstMouseMoveEvent = true;
}

}
