#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Core
{
	
class Renderer
{
public:

	static void ClearColor(const glm::vec4& color);

	static void ClearScreen();

	static void DrawLine(const glm::vec3& a, const glm::vec3& b);

private:

	void DrawLineImpl(const glm::vec3& a, const glm::vec3& b);

	friend class Application;

	Renderer() = default;
	static Renderer& GetRenderer(); // Returns Renderer instance from Application

	glm::mat4 m_view;
	glm::mat4 m_projection;
};

}
