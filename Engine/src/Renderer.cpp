#include <Renderer.h>

#include <Application.h>

#include <Renderer/BufferLayout.h>
#include <Renderer/CubemapTexture.h>
#include <Renderer/RenderCommand.h>
#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>

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

Renderer::Renderer()
{
	// no-op for now
}

bool Renderer::Initialize()
{
	// Set Renderer API to OpenGL
	SetAPI(RendererAPI::API::OpenGL);

	stbi_set_flip_vertically_on_load(true);

	// Create Engine Shaders
	m_modelShader = Shader::Create(kPositionNormalAndTexCoordVS, kColorFromTextureFS);
	m_lineShader = Shader::Create(kPositionAndColorVS, kColorFromVertexFS);
	m_cubeMapShader = Shader::Create(kCubeMapVS, kCubeMapFS);

	// Create Engine Textures
	m_skyBoxTexture = std::make_shared<CubemapTexture>(kSkyboxRightTexture, kSkyboxLeftTexture, kSkyboxTopTexture, kSkyboxBottomTexture, kSkyboxFrontTexture, kSkyboxBackTexture);
	m_spaceSkyBoxTexture = std::make_shared<CubemapTexture>(kSpaceSkyboxRightTexture, kSpaceSkyboxLeftTexture, kSpaceSkyboxTopTexture, kSpaceSkyboxBottomTexture, kSpaceSkyboxFrontTexture, kSpaceSkyboxBackTexture);

	return true;
}


void Renderer::BeginScene(const SceneData& sceneData)
{
	GetRenderer().BeginSceneImpl(sceneData);
}

void Renderer::EndScene()
{
	GetRenderer().EndSceneImpl();
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

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
	RendererAPI::DrawMode drawMode, const glm::mat4& transform)
{
	GetRenderer().SubmitImpl(shader, vertexArray, drawMode, transform);
}

void Renderer::BeginSceneImpl(const SceneData& sceneData)
{
	// Set the scene data
	m_sceneData = sceneData;
}

void Renderer::EndSceneImpl()
{
	// Clear the scene data
	m_sceneData = SceneData();
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

	unsigned int indices[] = { 0, 1 };

	auto vao = VertexArray::Create();
	vao->Bind();

	BufferLayout layout{
		{
		{ShaderDataType::Float3, "Position"},
		{ShaderDataType::Float3, "Color"}
		}
	};

	auto vbo = VertexBuffer::Create(vertices, sizeof(vertices));
	vbo->SetLayout(layout);
	vao->AddVertexBuffer(vbo);

	auto ebo = ElementBuffer::Create(indices, sizeof(indices));
	vao->SetElementBuffer(ebo);

	Submit(m_lineShader, vao, RendererAPI::DrawMode::Lines);
}

void Renderer::DrawSkyboxImpl()
{
	// Define 8 unique vertices (the corners of the cube)
	float skyboxVertices[] = {
		// positions          
		-1.0f, -1.0f, -1.0f,  // 0
		 1.0f, -1.0f, -1.0f,  // 1
		 1.0f,  1.0f, -1.0f,  // 2
		-1.0f,  1.0f, -1.0f,  // 3
		-1.0f, -1.0f,  1.0f,  // 4
		 1.0f, -1.0f,  1.0f,  // 5
		 1.0f,  1.0f,  1.0f,  // 6
		-1.0f,  1.0f,  1.0f   // 7
	};

	// Index buffer for the 6 faces (2 triangles each = 36 indices)
	unsigned int skyboxIndices[] = {
		// Back face (z = -1)
		3, 0, 1,
		1, 2, 3,

		// Left face (x = -1)
		4, 0, 3,
		3, 7, 4,

		// Right face (x = 1)
		1, 5, 6,
		6, 2, 1,

		// Front face (z = 1)
		4, 7, 6,
		6, 5, 4,

		// Top face (y = 1)
		3, 2, 6,
		6, 7, 3,

		// Bottom face (y = -1)
		0, 4, 1,
		1, 4, 5
	};

	auto vao = VertexArray::Create();
	vao->Bind();

	BufferLayout layout{
		{
			{ShaderDataType::Float3, "Position"}
		}
	};

	auto vbo = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
	vbo->SetLayout(layout);
	vao->AddVertexBuffer(vbo);

	auto ebo = ElementBuffer::Create(skyboxIndices, sizeof(skyboxIndices));
	vao->SetElementBuffer(ebo);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(false);
	m_skyBoxTexture->Bind();

	Submit(m_cubeMapShader, vao, RendererAPI::DrawMode::Triangles);

	m_skyBoxTexture->Unbind();
	glDepthMask(true);
	glDepthFunc(GL_LESS);

}

void Renderer::DrawSpaceSkyboxImpl()
{
	// Define 8 unique vertices (the corners of the cube)
	float skyboxVertices[] = {
		// positions          
		-1.0f, -1.0f, -1.0f,  // 0
		 1.0f, -1.0f, -1.0f,  // 1
		 1.0f,  1.0f, -1.0f,  // 2
		-1.0f,  1.0f, -1.0f,  // 3
		-1.0f, -1.0f,  1.0f,  // 4
		 1.0f, -1.0f,  1.0f,  // 5
		 1.0f,  1.0f,  1.0f,  // 6
		-1.0f,  1.0f,  1.0f   // 7
	};

	// Index buffer for the 6 faces (2 triangles each = 36 indices)
	unsigned int skyboxIndices[] = {
		// Back face (z = -1)
		3, 0, 1,
		1, 2, 3,

		// Left face (x = -1)
		4, 0, 3,
		3, 7, 4,

		// Right face (x = 1)
		1, 5, 6,
		6, 2, 1,

		// Front face (z = 1)
		4, 7, 6,
		6, 5, 4,

		// Top face (y = 1)
		3, 2, 6,
		6, 7, 3,

		// Bottom face (y = -1)
		0, 4, 1,
		1, 4, 5
	};

	auto vao = VertexArray::Create();
	vao->Bind();

	BufferLayout layout{
		{
			{ShaderDataType::Float3, "Position"}
		}
	};

	auto vbo = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
	vbo->SetLayout(layout);
	vao->AddVertexBuffer(vbo);

	auto ebo = ElementBuffer::Create(skyboxIndices, sizeof(skyboxIndices));
	vao->SetElementBuffer(ebo);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(false);
	m_spaceSkyBoxTexture->Bind();

	Submit(m_cubeMapShader, vao, RendererAPI::DrawMode::Triangles);

	m_spaceSkyBoxTexture->Unbind();
	glDepthMask(true);
	glDepthFunc(GL_LESS);
}

void Renderer::SubmitImpl(const Model& model, const glm::mat4& transform)
{
	m_modelShader->Bind();
	m_modelShader->SetUniformMatrix4fv(kModelMatrixUniform, glm::value_ptr(transform));
	m_modelShader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_sceneData.ViewMatrix));
	m_modelShader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_sceneData.ProjectionMatrix));
	model.Draw(*m_modelShader);
}

void Renderer::SubmitImpl(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, RendererAPI::DrawMode drawMode, const glm::mat4& transform)
{
	shader->Bind();
	shader->SetUniformMatrix4fv(kModelMatrixUniform, glm::value_ptr(transform));
	shader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_sceneData.ViewMatrix));
	shader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_sceneData.ProjectionMatrix));

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray, drawMode);
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
