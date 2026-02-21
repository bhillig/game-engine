#include <Renderer/Shader.h>

#include <Renderer.h>

#include <Platform/OpenGL/OpenGLShader.h>

namespace Core
{

Ref<Shader> Shader::Create(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath)
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

bool ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
	if (m_shaders.contains(name))
	{
		LOG_CORE_WARN("Shader with name '{}' already exists!", name);
		return false;
	}
	m_shaders[name] = shader;
	return true;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, std::string_view vertexShaderFilePath,
	std::string_view fragmentShaderFilePath)
{
	auto shader = Shader::Create(vertexShaderFilePath, fragmentShaderFilePath);
	Add(name, shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
	assert(m_shaders.contains(name) && "Shader not found!");
	return m_shaders[name];
}

}
