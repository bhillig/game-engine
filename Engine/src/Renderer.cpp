#include <Renderer.h>

#include <Application.h>

#include <Renderer/BufferLayout.h>
#include <Renderer/CubemapTexture.h>
#include <Renderer/RenderCommand.h>
#include <Renderer/Renderer2D.h>
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

// Uniform constants
static constexpr const char* kModelMatrixUniform = "u_Model";
static constexpr const char* kViewMatrixUniform = "u_View";
static constexpr const char* kProjectionMatrixUniform = "u_Projection";

namespace Core
{

struct RendererInfo
{
	Ref<VertexArray> skyBoxVA;
	Ref<CubemapTexture> skyBoxTexture;
	Ref<Shader> skyBoxShader;

	Ref<Shader> modelShader;
	Ref<Shader> lineShader;
};

static RendererInfo s_rendererInfo;

Renderer::Renderer()
{
	// no-op for now
}

bool Renderer::Initialize()
{
	// Set Renderer API to OpenGL
	SetAPI(RendererAPI::API::OpenGL);

	RenderCommand::Init();

	Renderer2D::Init();

	stbi_set_flip_vertically_on_load(true);

	// Create Engine Shaders
	s_rendererInfo.modelShader = m_shaderLibrary.Load("ModelShader", kPositionNormalAndTexCoordVS, kColorFromTextureFS);
	s_rendererInfo.lineShader = m_shaderLibrary.Load("LineShader", kPositionAndColorVS, kColorFromVertexFS);
	s_rendererInfo.skyBoxShader = m_shaderLibrary.Load("CubemapShader", kCubeMapVS, kCubeMapFS);

	// Create Engine Textures
	s_rendererInfo.skyBoxTexture = CubemapTexture::Create(kSkyboxRightTexture, kSkyboxLeftTexture, kSkyboxTopTexture, kSkyboxBottomTexture, kSkyboxFrontTexture, kSkyboxBackTexture);

	// Initialize Skybox Vertex Array

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

	s_rendererInfo.skyBoxVA = VertexArray::Create();

	auto& vao = s_rendererInfo.skyBoxVA;
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

ShaderLibrary& Renderer::GetShaderLibrary()
{
	return GetRenderer().GetShaderLibraryImpl();
}

void Renderer::DrawBoundingBox(const glm::mat4& transform, const glm::vec3& min, const glm::vec3& max)
{
	GetRenderer().DrawBoundingBoxImpl(transform, min, max);
}

void Renderer::DrawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color)
{
	GetRenderer().DrawLineImpl(a, b, color);
}

void Renderer::DrawBillboard(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	GetRenderer().DrawBillboardImpl(position, size, color);
}

void Renderer::DrawBillboard(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)
{
	GetRenderer().DrawBillboardImpl(position, size, texture);
}

void Renderer::DrawSkybox()
{
	GetRenderer().DrawSkyboxImpl();
}

void Renderer::Submit(const Model& model, const glm::mat4& transform)
{
	GetRenderer().SubmitImpl(model, transform);
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
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

void Renderer::DrawBoundingBoxImpl(const glm::mat4& transform, const glm::vec3& min, const glm::vec3& max)
{

	const glm::vec3 worldMin = glm::vec3(transform * glm::vec4(min, 1.0));
	const glm::vec3 worldMax = glm::vec3(transform * glm::vec4(max, 1.0));

	// Calculate all lines that need to be drawn
	const glm::vec3 bottomLeftNear = worldMin;
	const glm::vec3 bottomRightNear = glm::vec3(worldMax.x, worldMin.y, worldMin.z);
	const glm::vec3 bottomLeftFar = glm::vec3(worldMin.x, worldMin.y, worldMax.z);
	const glm::vec3 bottomRightFar = glm::vec3(worldMax.x, worldMin.y, worldMax.z);

	const glm::vec3 topLeftNear = glm::vec3(worldMin.x, worldMax.y, worldMin.z);
	const glm::vec3 topRightNear = glm::vec3(worldMax.x, worldMax.y, worldMin.z);
	const glm::vec3 topLeftFar = glm::vec3(worldMin.x, worldMax.y, worldMax.z);
	const glm::vec3 topRightFar = worldMax;

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

	unsigned int indices[] = { 0, 1, 2, };

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

	Submit(s_rendererInfo.lineShader, vao, RendererAPI::DrawMode::Lines);
}

void Renderer::DrawBillboardImpl(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	const glm::mat4& viewMatrix = m_sceneData.ViewMatrix;
	const float halfWidth = size.x * 0.5f;
	const float halfHeight = size.y * 0.5f;

	// Calculate transform
	const glm::vec3 right = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	const glm::vec3 up = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

	const glm::vec3 bottomLeft = position - right * halfWidth - up * halfHeight;
	const glm::vec3 bottomRight = position + right * halfWidth - up * halfHeight;
	const glm::vec3 topRight = position + right * halfWidth + up * halfHeight;
	const glm::vec3 topLeft = position - right * halfWidth + up * halfHeight;

	float vertices[] = {
		bottomLeft.x, bottomLeft.y, bottomLeft.z, color.r, color.g, color.b,
		bottomRight.x, bottomRight.y, bottomRight.z, color.r, color.g, color.b,
		topRight.x, topRight.y, topRight.z, color.r, color.g, color.b,
		topLeft.x, topLeft.y, topLeft.z, color.r, color.g, color.b,
	};

	unsigned int indices[] = { 0, 1, 2, 2, 3, 0};

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

	auto lineShader = m_shaderLibrary.Get("LineShader");
	Submit(lineShader, vao, RendererAPI::DrawMode::Triangles);
}

void Renderer::DrawBillboardImpl(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)
{
	const glm::mat4& viewMatrix = m_sceneData.ViewMatrix;
	const float halfWidth = size.x * 0.5f;
	const float halfHeight = size.y * 0.5f;

	// Calculate transform
	const glm::vec3 right = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	const glm::vec3 up = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

	const glm::vec3 bottomLeft = position - right * halfWidth - up * halfHeight;
	const glm::vec3 bottomRight = position + right * halfWidth - up * halfHeight;
	const glm::vec3 topRight = position + right * halfWidth + up * halfHeight;
	const glm::vec3 topLeft = position - right * halfWidth + up * halfHeight;

	float vertices[] = {
		bottomLeft.x, bottomLeft.y, bottomLeft.z, 0.0, 0.0,
		bottomRight.x, bottomRight.y, bottomRight.z, 1.0, 0.0,
		topRight.x, topRight.y, topRight.z, 1.0, 1.0,
		topLeft.x, topLeft.y, topLeft.z, 0.0, 1.0,
	};

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0
	};

	auto vao = VertexArray::Create();
	vao->Bind();

	auto layout = BufferLayout{
		{
			{ShaderDataType::Float3, "Position"},
			{ShaderDataType::Float2, "TexCoord"}
		}};

	auto vbo = VertexBuffer::Create(vertices, sizeof(vertices));
	vbo->SetLayout(layout);
	vao->AddVertexBuffer(vbo);

	auto ebo = ElementBuffer::Create(indices, sizeof(indices));
	vao->SetElementBuffer(ebo);

	texture->Bind();

	auto posTexShader = GetShaderLibrary().Get("PosTexShader");
	Submit(posTexShader, vao, RendererAPI::DrawMode::Triangles);
}

void Renderer::DrawSkyboxImpl()
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(false);

	s_rendererInfo.skyBoxTexture->Bind(0);
	Submit(s_rendererInfo.skyBoxShader, s_rendererInfo.skyBoxVA, RendererAPI::DrawMode::Triangles);

	glDepthMask(true);
	glDepthFunc(GL_LESS);
}

void Renderer::SubmitImpl(const Model& model, const glm::mat4& transform)
{
	auto modelShader = m_shaderLibrary.Get("ModelShader");
	modelShader->Bind();
	modelShader->SetUniformMatrix4fv(kModelMatrixUniform, glm::value_ptr(transform));
	modelShader->SetUniformMatrix4fv(kViewMatrixUniform, glm::value_ptr(m_sceneData.ViewMatrix));
	modelShader->SetUniformMatrix4fv(kProjectionMatrixUniform, glm::value_ptr(m_sceneData.ProjectionMatrix));
	model.Draw(*modelShader);
}

void Renderer::SubmitImpl(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, RendererAPI::DrawMode drawMode, const glm::mat4& transform)
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
