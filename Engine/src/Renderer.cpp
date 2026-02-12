#include <Renderer.h>

#include <Application.h>

#include <Renderer/CubemapTexture.h>
#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

// Vertex Shaders
static constexpr std::string_view kPositionNormalAndTexCoordVS = SHADER_DIR "/positionNormalAndTexCoords.vertex.glsl";
static constexpr std::string_view kPositionAndColorVS = SHADER_DIR "/positionAndColor.vertex.glsl";
static constexpr std::string_view kCubeMapVS = SHADER_DIR "/cubemap.vertex.glsl";

// Fragment Shaders
static constexpr std::string_view kColorFromTextureFS = SHADER_DIR "/colorFromTexture.fragment.glsl";
static constexpr std::string_view kColorFromVertexFS = SHADER_DIR "/colorFromVertex.fragment.glsl";
static constexpr std::string_view kCubeMapFS = SHADER_DIR "/cubemap.fragment.glsl";

// Skybox Textures
static constexpr std::string_view kSkyboxRightTexture = TEXTURE_DIR "/skybox/skybox-right.jpg";
static constexpr std::string_view kSkyboxLeftTexture = TEXTURE_DIR "/skybox/skybox-left.jpg";
static constexpr std::string_view kSkyboxTopTexture = TEXTURE_DIR "/skybox/skybox-top.jpg";
static constexpr std::string_view kSkyboxBottomTexture = TEXTURE_DIR "/skybox/skybox-bottom.jpg";
static constexpr std::string_view kSkyboxBackTexture = TEXTURE_DIR "/skybox/skybox-back.jpg";
static constexpr std::string_view kSkyboxFrontTexture = TEXTURE_DIR "/skybox/skybox-front.jpg";

// Space Skybox Textures
static constexpr std::string_view kSpaceSkyboxRightTexture = TEXTURE_DIR "/space-skybox/right.png";
static constexpr std::string_view kSpaceSkyboxLeftTexture = TEXTURE_DIR "/space-skybox/left.png";
static constexpr std::string_view kSpaceSkyboxTopTexture = TEXTURE_DIR "/space-skybox/top.png";
static constexpr std::string_view kSpaceSkyboxBottomTexture = TEXTURE_DIR "/space-skybox/bottom.png";
static constexpr std::string_view kSpaceSkyboxBackTexture = TEXTURE_DIR "/space-skybox/back.png";
static constexpr std::string_view kSpaceSkyboxFrontTexture = TEXTURE_DIR "/space-skybox/front.png";

// Uniform constants
static constexpr const char* kModelMatrixUniform = "u_Model";
static constexpr const char* kViewMatrixUniform = "u_View";
static constexpr const char* kProjectionMatrixUniform = "u_Projection";

namespace Core
{

RendererAPI Renderer::s_rendererAPI = RendererAPI::None;

Renderer::Renderer()
{
	// no-op for now
}

bool Renderer::Initialize()
{
	stbi_set_flip_vertically_on_load(true);

	// Create Engine Shaders
	m_modelShader = std::make_unique<Shader>(kPositionNormalAndTexCoordVS, kColorFromTextureFS);
	m_lineShader = std::make_unique<Shader>(kPositionAndColorVS, kColorFromVertexFS);
	m_cubeMapShader = std::make_unique<Shader>(kCubeMapVS, kCubeMapFS);

	// Create Engine Textures
	m_skyBoxTexture = std::make_unique<CubemapTexture>(kSkyboxRightTexture, kSkyboxLeftTexture, kSkyboxTopTexture, kSkyboxBottomTexture, kSkyboxFrontTexture, kSkyboxBackTexture);
	m_spaceSkyBoxTexture = std::make_unique<CubemapTexture>(kSpaceSkyboxRightTexture, kSpaceSkyboxLeftTexture, kSpaceSkyboxTopTexture, kSpaceSkyboxBottomTexture, kSpaceSkyboxFrontTexture, kSpaceSkyboxBackTexture);

	// Set Renderer API to OpenGL
	SetRendererAPI(RendererAPI::OpenGL);

	return true;
}

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

void Renderer::DrawSkybox()
{
	GetRenderer().DrawSkyboxImpl();
}

void Renderer::DrawSpaceSkybox()
{
	GetRenderer().DrawSpaceSkyboxImpl();
}

void Renderer::Submit(const Model& model, const glm::mat4& transform)
{
	GetRenderer().SubmitImpl(model, transform);
}

void Renderer::SetRendererAPI(RendererAPI api)
{
	s_rendererAPI = api;
}

RendererAPI Renderer::GetAPI()
{
	return s_rendererAPI;
}

void Renderer::BeginSceneImpl(const glm::mat4& view, const glm::mat4& projection)
{
	m_view = view;
	m_modelShader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_view));
	m_lineShader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_view));
	m_cubeMapShader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_view));

	m_projection = projection;
	m_modelShader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_projection));
	m_lineShader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_projection));
	m_cubeMapShader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_projection));
}

void Renderer::EndSceneImpl()
{
	m_view = glm::mat4(1);
	m_modelShader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_view));
	m_lineShader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_view));
	m_cubeMapShader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_view));

	m_projection = glm::mat4(1);
	m_modelShader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_projection));
	m_lineShader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_projection));
	m_cubeMapShader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_projection));
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

	auto vbo = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));

	VertexArray vao;
	vao.Add(*vbo, layout);

	glm::mat4 transform = glm::mat4(1.f);
	m_lineShader->Bind();
	m_lineShader->SetUniformMatrix4fv(kModelMatrixUniform, glm::value_ptr(transform));

	vao.Bind();

	glDrawArrays(GL_LINES, 0, 2);

	m_lineShader->Unbind();
	vao.Unbind();
}

void Renderer::DrawSkyboxImpl()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);
	
	auto vbo = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices)));

	VertexArray vao;
	vao.Add(*vbo, layout);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(false);
	m_cubeMapShader->Bind();
	vao.Bind();
	m_skyBoxTexture->Bind();

	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_cubeMapShader->Unbind();
	vao.Unbind();
	m_skyBoxTexture->Unbind();
	glDepthMask(true);
	glDepthFunc(GL_LESS);

}

void Renderer::DrawSpaceSkyboxImpl()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);

	auto vbo = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices)));

	VertexArray vao;
	vao.Add(*vbo, layout);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(false);
	m_cubeMapShader->Bind();
	vao.Bind();
	m_spaceSkyBoxTexture->Bind();

	glDrawArrays(GL_TRIANGLES, 0, 36);

	m_cubeMapShader->Unbind();
	vao.Unbind();
	m_spaceSkyBoxTexture->Unbind();
	glDepthMask(true);
	glDepthFunc(GL_LESS);
}

void Renderer::SubmitImpl(const Model& model, const glm::mat4& transform)
{
	glm::mat4 modelTransform = transform;
	m_modelShader->Bind();
	m_modelShader->SetUniformMatrix4fv(kModelMatrixUniform, glm::value_ptr(modelTransform));
	model.Draw(*m_modelShader);
	m_modelShader->Unbind();
}

Renderer::~Renderer()
{
	// no-op for now
}

Renderer& Renderer::GetRenderer()
{
	return Application::GetApp()->GetRenderer();
}

}
