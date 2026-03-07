#include <Renderer/Renderer2D.h>

#include <Renderer.h>

#include <Renderer/BufferLayout.h>
#include <Renderer/RenderCommand.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexArray.h>

#include <OrthographicCamera.h>

// Vertex Shaders
static constexpr std::string_view kPositionVS = SHADER_DIR "/position.vertex.glsl";
static constexpr std::string_view kPositionAndTexCoordsVS = SHADER_DIR "/positionAndTexCoords.vertex.glsl";

// Fragment Shaders
static constexpr std::string_view kColorFromUniformFS = SHADER_DIR "/colorFromUniform.fragment.glsl";
static constexpr std::string_view kColorFromTextureFS = SHADER_DIR "/colorFromTexture.fragment.glsl";

// Textures
static constexpr std::string_view kWhiteTexture = TEXTURE_DIR "/white.png";

namespace Core
{

struct Renderer2DInfo
{
	Ref<VertexArray> va;
	Ref<Texture> whiteTexture;
	Ref<Shader> colorAndTextureShader;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};

static Renderer2DInfo s_Renderer2DInfo;

void Renderer2D::Init()
{
	LOG_CORE_INFO("Renderer2D Initialized");

	// Load White Texture
	// TODO: Optimization. Create a constructor that allows you to create a texture with just a width and height.
	// Create a setData function that sets the inner contents of a texture. Pass in 0xffffffff directly.
	s_Renderer2DInfo.whiteTexture = Texture::Create(std::string(kWhiteTexture));

	// Define vertex info
	float vertices[]{
		-0.5f, -0.5f, 0.f, 0.f, 0.f,
		0.5f, -0.5f, 0.f, 1.0f, 0.f,
		0.5f, 0.5f, 0.f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.f, 0.f, 1.0f
	};

	unsigned int indices[]{
		0, 1, 2, 2, 3, 0
	};

	// Create vertex array
	s_Renderer2DInfo.va = VertexArray::Create();
	s_Renderer2DInfo.va->Bind();

	auto& va = s_Renderer2DInfo.va;

	BufferLayout layout{
		{
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float2, "a_TexCoord"}
		}
	};

	// Create vertex buffer
	auto vbo = VertexBuffer::Create(vertices, sizeof(vertices));
	vbo->SetLayout(layout);
	va->AddVertexBuffer(vbo);

	// Create element buffer
	auto ebo = ElementBuffer::Create(indices, sizeof(indices));
	va->SetElementBuffer(ebo);

	// Load Texture Shader
	s_Renderer2DInfo.colorAndTextureShader = Renderer::GetShaderLibrary().Load("Texture", kPositionAndTexCoordsVS, kColorFromTextureFS);
	s_Renderer2DInfo.colorAndTextureShader->Bind();
	s_Renderer2DInfo.colorAndTextureShader->SetUniform1i("u_Material.texture_diffuse1", 0);
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
	// Save view and projection matrices
	s_Renderer2DInfo.viewMatrix = camera.viewMatrix();
	s_Renderer2DInfo.projectionMatrix = camera.projectionMatrix();

	// Bind shader and set uniforms
	s_Renderer2DInfo.colorAndTextureShader->Bind();
	s_Renderer2DInfo.colorAndTextureShader->SetUniformMatrix4fv("u_View", glm::value_ptr(s_Renderer2DInfo.viewMatrix));
	s_Renderer2DInfo.colorAndTextureShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(s_Renderer2DInfo.projectionMatrix));
}

void Renderer2D::EndScene()
{
}

void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
{
	DrawQuad({ position.x, position.y, 0.f }, rotation, scale, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
{
	const glm::mat4 transform = glm::translate(glm::mat4(1.f), position) 
	* glm::rotate(glm::mat4(1.f), rotation, glm::vec3{ 0.f, 0.f, 1.0f }) 
	* glm::scale(glm::mat4(1.f), { scale.x, scale.y, 1.f });

	// Set color uniform
	s_Renderer2DInfo.colorAndTextureShader->SetUniform4f("u_Color", color.r, color.g, color.b, color.a);
	
	// Set white texture
	s_Renderer2DInfo.whiteTexture->Bind();

	// Set tiling factor
	s_Renderer2DInfo.colorAndTextureShader->SetUniform1f("u_TilingFactor", 1.f);

	// Set transform
	s_Renderer2DInfo.colorAndTextureShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(transform));

	s_Renderer2DInfo.va->Bind();
	RenderCommand::DrawIndexed(s_Renderer2DInfo.va);
}

void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const Ref<Texture>& texture, float tilingFactor)
{
	DrawQuad({ position.x, position.y, 0.f }, rotation, scale, texture, tilingFactor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const Ref<Texture>& texture, float tilingFactor)
{
	const glm::mat4 transform = glm::translate(glm::mat4(1.f), position) 
	* glm::rotate(glm::mat4(1.f), rotation, glm::vec3{ 0.f, 0.f, 1.0f })
	* glm::scale(glm::mat4(1.f), { scale.x, scale.y, 1.f });

	// Set color uniform to white
	s_Renderer2DInfo.colorAndTextureShader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	// Set texture
	texture->Bind();

	// Set tiling factor
	s_Renderer2DInfo.colorAndTextureShader->SetUniform1f("u_TilingFactor", tilingFactor);

	// Set transform
	s_Renderer2DInfo.colorAndTextureShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(transform));

	s_Renderer2DInfo.va->Bind();
	RenderCommand::DrawIndexed(s_Renderer2DInfo.va);
}

}
