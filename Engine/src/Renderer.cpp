#include <Renderer.h>

#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <glad/glad.h>

#include <Application.h>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shaders
static constexpr std::string_view kPositionNormalAndTexCoordVS = SHADER_DIR "/positionNormalAndTexCoords.vertex.glsl";

// Fragment Shaders
static constexpr std::string_view kColorFromTextureFS = SHADER_DIR "/colorFromTexture.fragment.glsl";

namespace Core
{

void Renderer::BeginScene(const glm::mat4& view, const glm::mat4& projection)
{
	GetRenderer().BeginSceneImpl(view, projection);
}

void Renderer::ClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawLine(const glm::vec3& a, const glm::vec3& b)
{
	GetRenderer().DrawLineImpl(a, b);
}

void Renderer::Submit(const Model& model, const glm::mat4& transform)
{
	GetRenderer().SubmitImpl(model, transform);
}

void Renderer::Initialize()
{
	m_modelShader = std::make_unique<Shader>(kPositionNormalAndTexCoordVS, kColorFromTextureFS);
}

void Renderer::BeginSceneImpl(const glm::mat4& view, const glm::mat4& projection)
{
	m_view = view;
	m_modelShader->SetUniformMatrix4fv("u_View", glm::value_ptr(m_view));

	m_projection = projection;
	m_modelShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(m_projection));
}

void Renderer::DrawLineImpl(const glm::vec3& a, const glm::vec3& b)
{
	float vertices[] = {
		a.x, a.y, a.z,
		b.x, b.y, b.z
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);

	VertexBuffer vbo(vertices, sizeof(vertices));

	VertexArray vao;
	vao.Add(vbo, layout);

	glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0);
}

void Renderer::SubmitImpl(const Model& model, const glm::mat4& transform)
{
	glm::mat4 modelTransform = transform;
	m_modelShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(modelTransform));
	model.Draw(*m_modelShader);
}

Renderer& Renderer::GetRenderer()
{
	return Application::GetApp()->GetRenderer();
}

}
