#include <Renderer.h>

#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <glad/glad.h>

#include <Application.h>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shaders
static constexpr std::string_view kPositionNormalAndTexCoordVS = SHADER_DIR "/positionNormalAndTexCoords.vertex.glsl";
static constexpr std::string_view kPositionAndColorVS = SHADER_DIR "/positionAndColor.vertex.glsl";

// Fragment Shaders
static constexpr std::string_view kColorFromTextureFS = SHADER_DIR "/colorFromTexture.fragment.glsl";
static constexpr std::string_view kColorFromVertexFS = SHADER_DIR "/colorFromVertex.fragment.glsl";

namespace Core
{

void Renderer::BeginScene(const glm::mat4& view, const glm::mat4& projection)
{
	GetRenderer().BeginSceneImpl(view, projection);
}

void Renderer::EndScene()
{
	GetRenderer().EndSceneImpl();
}

void Renderer::ClearColor(const glm::vec4& color)
{
	GetRenderer().ClearColorImpl(color);
}

void Renderer::ClearScreen()
{
	GetRenderer().ClearScreenImpl();
}

void Renderer::DrawBoundingBox(const glm::vec3& min, const glm::vec3& max)
{
	GetRenderer().DrawBoundingBoxImpl(min, max);
}

void Renderer::DrawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color)
{
	GetRenderer().DrawLineImpl(a, b, color);
}

void Renderer::Submit(const Model& model, const glm::mat4& transform)
{
	GetRenderer().SubmitImpl(model, transform);
}

void Renderer::Initialize()
{
	m_modelShader = std::make_unique<Shader>(kPositionNormalAndTexCoordVS, kColorFromTextureFS);
	m_lineShader = std::make_unique<Shader>(kPositionAndColorVS, kColorFromVertexFS);
}

void Renderer::BeginSceneImpl(const glm::mat4& view, const glm::mat4& projection)
{
	m_view = view;
	m_modelShader->SetUniformMatrix4fv("u_View", glm::value_ptr(m_view));
	m_lineShader->SetUniformMatrix4fv("u_View", glm::value_ptr(m_view));

	m_projection = projection;
	m_modelShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(m_projection));
	m_lineShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(m_projection));
}

void Renderer::EndSceneImpl()
{
	m_view = glm::mat4(1);
	m_modelShader->SetUniformMatrix4fv("u_View", glm::value_ptr(m_view));
	m_lineShader->SetUniformMatrix4fv("u_View", glm::value_ptr(m_view));

	m_projection = glm::mat4(1);
	m_modelShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(m_projection));
	m_lineShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(m_projection));
}

void Renderer::ClearColorImpl(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::ClearScreenImpl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawBoundingBoxImpl(const glm::vec3& min, const glm::vec3& max)
{
	// Calculate all lines that need to be drawn
	const glm::vec3 bottomLeftNear = min;
	const glm::vec3 bottomRightNear = glm::vec3(max.x, min.y, min.z);
	const glm::vec3 bottomLeftFar = glm::vec3(min.x, min.y, max.z);
	const glm::vec3 bottomRightFar = glm::vec3(max.x, min.y, max.z);

	const glm::vec3 topLeftNear = glm::vec3(min.x, max.y, min.z);
	const glm::vec3 topRightNear = glm::vec3(max.x, max.y, min.z);
	const glm::vec3 topLeftFar = glm::vec3(min.x, max.y, max.z);
	const glm::vec3 topRightFar = max;

	// Draw bottom-face
	DrawLineImpl(bottomLeftNear, bottomRightNear);
	DrawLineImpl(bottomRightNear, bottomRightFar);
	DrawLineImpl(bottomRightFar, bottomLeftFar);
	DrawLineImpl(bottomLeftFar, bottomLeftNear);

	// Draw top-face
	DrawLineImpl(topLeftNear, topRightNear);
	DrawLineImpl(topRightNear, topRightFar);
	DrawLineImpl(topRightFar, topLeftFar);
	DrawLineImpl(topLeftFar, topLeftNear);

	// Draw vertical lines connecting the faces
	DrawLineImpl(bottomLeftNear, topLeftNear);
	DrawLineImpl(bottomRightNear, topRightNear);
	DrawLineImpl(bottomRightFar, topRightFar);
	DrawLineImpl(bottomLeftFar, topLeftFar);
}

void Renderer::DrawLineImpl(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color)
{
	float vertices[] = {
		a.x, a.y, a.z, color.r, color.g, color.b,
		b.x, b.y, b.z, color.r, color.g, color.b
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);

	VertexBuffer vbo(vertices, sizeof(vertices));

	VertexArray vao;
	vao.Add(vbo, layout);

	glm::mat4 transform = glm::mat4(1.f);
	m_lineShader->Bind();
	m_lineShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(transform));

	vao.Bind();

	glDrawArrays(GL_LINES, 0, 2);

	m_lineShader->Unbind();
	vao.Unbind();
}

void Renderer::SubmitImpl(const Model& model, const glm::mat4& transform)
{
	glm::mat4 modelTransform = transform;
	m_modelShader->Bind();
	m_modelShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(modelTransform));
	model.Draw(*m_modelShader);
	m_modelShader->Unbind();
}

Renderer& Renderer::GetRenderer()
{
	return Application::GetApp()->GetRenderer();
}

}
