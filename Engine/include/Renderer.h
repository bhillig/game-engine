#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class Model;
class Shader;

namespace Core
{
	
class Renderer
{
public:

	static void BeginScene(const glm::mat4& view, const glm::mat4& projection);

	static void ClearColor(const glm::vec4& color);

	static void ClearScreen();

	static void DrawLine(const glm::vec3& a, const glm::vec3& b);

	static void Submit(const Model& model, const glm::mat4& transform);

private:

	void Initialize();

	void BeginSceneImpl(const glm::mat4& view, const glm::mat4& projection);

	void DrawLineImpl(const glm::vec3& a, const glm::vec3& b);

	void SubmitImpl(const Model& model, const glm::mat4& transform);

	friend class Application;

	Renderer() = default;
	static Renderer& GetRenderer(); // Returns Renderer instance from Application

	glm::mat4 m_view;
	glm::mat4 m_projection;

	std::unique_ptr<Shader> m_modelShader;

};

}
