#pragma once

#include <Renderer/Model.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ECS
{

class Component
{
public:
	virtual ~Component() {}

	void SetActive(bool active) { m_active = active; }
	bool IsActive() const { return m_active; }

private:
	bool m_active = false;
};

class TransformComponent : public Component
{
public:
	TransformComponent()
		: position(0.0, 0.0, 0.0)
		, rotation(0.f, 0.f, 0.f)
		, scale(1.f, 1.f, 1.f) {}

	glm::mat4 transformMatrix() const
	{
		glm::mat4 transform(1.f);
		transform = glm::translate(transform, position);
		transform = glm::rotate(transform, rotation.x, glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1));
		transform = glm::scale(transform, scale);
		return transform;
	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class MeshComponent : public Component
{
public:
	MeshComponent() 
	: m_model(nullptr){}
	MeshComponent(Core::Model* model)
		: m_model(model) {}

	void SetModel(Core::Model* model) { m_model = model; }

	Core::Model* GetModel() const { return m_model; }

private:
	Core::Model* m_model;
};

class BoundingBoxComponent : public Component
{
public:
	BoundingBoxComponent()  : m_min(0.f), m_max(0.f) {};
	BoundingBoxComponent(const glm::vec3& min, const glm::vec3& max)
		: m_min(min), m_max(max) {}

	void SetCenter(const glm::vec3& center)
	{
		const glm::vec3 extents = GetExtents();
		m_min = center - extents;
		m_max = center + extents;
	}

	void SetExtents(const glm::vec3& extents)
	{
		const glm::vec3 center = GetCenter();
		m_min = center - extents;
		m_max = center + extents;
	}

	void SetSize(const glm::vec3& size)
	{
		SetExtents(size * 0.5f);
	}

	glm::vec3 GetCenter() const { return (m_min + m_max) * 0.5f; }

	glm::vec3 GetExtents() const { return (m_max - m_min) * 0.5f; }

	glm::vec3 GetSize() const { return m_max - m_min; }

	const glm::vec3& GetMin() const { return m_min; }

	const glm::vec3& GetMax() const { return m_max; }

private:
	glm::vec3 m_min;
	glm::vec3 m_max;
};

} // namespace ECS