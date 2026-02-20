#include <Renderer/Shader.h>

#include <Renderer.h>

#include <Platform/OpenGL/OpenGLShader.h>

namespace Core
{

std::shared_ptr<Shader> Shader::Create(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return std::make_shared<OpenGLShader>(vertexShaderFilePath, fragmentShaderFilePath);
	default:
		break;
	}

	assert(false && "RendererAPI not set when trying to create Shader!");
	return nullptr;
}

}
