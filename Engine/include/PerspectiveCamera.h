#pragma once

#include <glm/glm.hpp>

namespace Core
{

class PerspectiveCamera
{
public:
	PerspectiveCamera(const glm::vec3& pos, float pitch, float yaw, float roll, float fov);

	const glm::vec3& position() const { return m_position; }
	glm::vec3 forward() const;
	glm::vec3 rotation() const { return { m_pitch, m_yaw, m_roll }; }
	glm::mat4 projectionMatrix(float aspectRatio) const;
	glm::mat4 viewMatrix() const;
	float fov() const { return m_fov; }

	void Move(const glm::vec3& offset);

	void Rotate(float pitchOffset, float yawOffset, float rollOffset);

	void SetFOV(float fov);

private:
	glm::vec3 m_position;
	float m_pitch;
	float m_yaw;
	float m_roll;
	float m_fov;
};

}