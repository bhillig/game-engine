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

class AABB
{
public:
	AABB() : min(0.f), max(0.f) {}
	AABB(const glm::vec3& min, const glm::vec3& max) 
		: min(min), max(max) {}

	glm::vec3 min;
	glm::vec3 max;
};


class BoundingBoxComponent : public Component
{
public:
	BoundingBoxComponent() {}
	BoundingBoxComponent(const glm::vec3& min, const glm::vec3& max)
		: m_boundingBox(min, max)
	{
	}

	void SetCenter(const glm::vec3& center)
	{
		const glm::vec3 extents = GetExtents();
		m_boundingBox.min = center - extents;
		m_boundingBox.max = center + extents;
	}

	void SetExtents(const glm::vec3& extents)
	{
		const glm::vec3 center = GetCenter();
		m_boundingBox.min = center - extents;
		m_boundingBox.max = center + extents;
	}

	void SetSize(const glm::vec3& size)
	{
		SetExtents(size * 0.5f);
	}

	glm::vec3 GetCenter() const { return (m_boundingBox.min + m_boundingBox.max) * 0.5f; }

	glm::vec3 GetExtents() const { return (m_boundingBox.max - m_boundingBox.max) * 0.5f; }

	glm::vec3 GetSize() const { return m_boundingBox.max - m_boundingBox.min; }

	const glm::vec3& GetMin() const { return m_boundingBox.min; }

	const glm::vec3& GetMax() const { return m_boundingBox.max; }

private:
	AABB m_boundingBox;
};

class LifeSpanComponent : public Component
{
public:
	LifeSpanComponent(float duration) : m_totalLifeSpan(duration), m_lifeSpanRemaining(duration) {}

	float GetLifeSpanRemaining() const { return m_lifeSpanRemaining; }

	float GetTotalLifeSpan() const { return m_totalLifeSpan; }

	void SetLifeSpanRemaining(float duration) { m_lifeSpanRemaining = duration; }

private:
	float m_totalLifeSpan;
	float m_lifeSpanRemaining;
};

} // namespace ECS