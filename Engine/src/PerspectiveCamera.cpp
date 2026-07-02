#include <PerspectiveCamera.h>

#include <algorithm>

namespace
{
	constexpr float kMinPitch = -89.f;
	constexpr float kMaxPitch = 89.f;
}

namespace Core
{

PerspectiveCamera::PerspectiveCamera(const glm::vec3& pos, float pitch, float yaw, float roll, float fov)
	: m_position(pos)
	, m_pitch(pitch)
	, m_yaw(yaw)
	, m_roll(roll)
	, m_fov(fov)
{
}

glm::vec3 PerspectiveCamera::forward() const
{
	glm::vec3 cameraForward(0.f);
	cameraForward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	cameraForward.y = sin(glm::radians(m_pitch));
	cameraForward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	return cameraForward;
}

glm::mat4 PerspectiveCamera::projectionMatrix(float aspectRatio) const
{
	return glm::perspectiveLH(glm::radians(fov()), aspectRatio, 0.1f, 100.f);
}

glm::mat4 PerspectiveCamera::viewMatrix() const
{
	return glm::lookAtLH(glm::vec3(m_position), // Camera location
		glm::vec3(m_position + forward()), // Target location
		glm::vec3(0.f, 1.0f, 0.f));	// World up direction
}

void PerspectiveCamera::Move(const glm::vec3& offset)
{
	m_position += offset;
}

void PerspectiveCamera::Rotate(float pitchOffset, float yawOffset, float rollOffset)
{
	m_pitch = std::clamp(m_pitch + pitchOffset, kMinPitch, kMaxPitch);
	m_yaw += yawOffset;
	m_roll += rollOffset;
}

void PerspectiveCamera::SetFOV(float fov)
{
	m_fov = fov;
}

}
