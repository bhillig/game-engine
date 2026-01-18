#include <CameraController.h>

#include <Application.h>

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CameraController::CameraController(Camera& camera)
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

CameraController::~CameraController()
{
}

void CameraController::Update(float deltaTime)
{
	const glm::vec3 cameraForward = m_camera.forward();
	const glm::vec3 walkDirection = glm::normalize(glm::vec3(cameraForward.x, 0.0f, cameraForward.z));
	const float movementSpeed = 15.f;
	const glm::vec3 upVector = glm::vec3(0.f, 1.0f, 0.f);

	glm::vec3 moveDelta(0.0f);
	if (m_forwardPressed)
	{
		moveDelta = movementSpeed * walkDirection * deltaTime;
	}
	if (m_backwardPressed)
	{
		moveDelta = -(movementSpeed * walkDirection * deltaTime);
	}
	if (m_leftPressed)
	{
		moveDelta = (glm::normalize(glm::cross(walkDirection, upVector)) * movementSpeed * deltaTime);
	}
	if (m_rightPressed)
	{
		moveDelta = -glm::normalize(glm::cross(walkDirection, upVector)) * movementSpeed * deltaTime;
	}

	m_camera.Move(moveDelta);
}

void CameraController::OnMouseMove(double xPos, double yPos)
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

	const float yawOffset = deltaX * m_mouseHorizontalSensitivity;
	const float pitchOffset = -deltaY * m_mouseVerticalSensitivity;

	m_camera.Rotate(pitchOffset, yawOffset, 0.f);

	lastX = xPos;
	lastY = yPos;
}

void CameraController::OnKeyPressed(int key)
{
	if (key == GLFW_KEY_W)
	{
		m_forwardPressed = true;
	}
	else if (key == GLFW_KEY_S)
	{
		m_backwardPressed = true;
	}
	else if (key == GLFW_KEY_A)
	{
		m_leftPressed = true;
	}
	else if (key == GLFW_KEY_D)
	{
		m_rightPressed = true;
	}
}

void CameraController::OnKeyReleased(int key)
{
	if (key == GLFW_KEY_W)
	{
		m_forwardPressed = false;
	}
	else if (key == GLFW_KEY_S)
	{
		m_backwardPressed = false;
	}
	else if (key == GLFW_KEY_A)
	{
		m_leftPressed = false;
	}
	else if (key == GLFW_KEY_D)
	{
		m_rightPressed = false;
	}
}

void CameraController::OnLoseControl()
{
	m_firstMouseMoveEvent = true;
}
