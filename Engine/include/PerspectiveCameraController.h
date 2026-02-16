#pragma once

#include <PerspectiveCamera.h>

namespace Core
{

class PerspectiveCameraController
{
public:
	PerspectiveCameraController(PerspectiveCamera& camera);
	~PerspectiveCameraController();

	void Update(float deltaTime);

	void OnMouseMove(double xPos, double yPos);

	void OnKeyPressed(int key);

	void OnKeyReleased(int key);

	void OnLoseControl();

private:
	PerspectiveCamera& m_camera;

	float m_mouseHorizontalSensitivity;
	float m_mouseVerticalSensitivity;

	bool m_forwardPressed;
	bool m_backwardPressed;
	bool m_leftPressed;
	bool m_rightPressed;

	bool m_firstMouseMoveEvent;
};

}
