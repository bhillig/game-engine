#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Renderer
{
	
	void ClearColor(const glm::vec4& color);

	void ClearScreen();

	void DrawLine(const glm::vec3& a, const glm::vec3& b);

}
