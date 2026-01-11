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

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class MeshComponent : public Component
{
public:
	MeshComponent() 
	: m_model(nullptr){}
	MeshComponent(Model* model)
		: m_model(model) {}

	void SetModel(Model* model) { m_model = model; }

	Model* GetModel() const { return m_model; }

private:
	Model* m_model;
};

} // namespace ECS