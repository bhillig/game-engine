#include <Renderer/Texture.h>

#include <Renderer.h>

#include <Platform/OpenGL/OpenGLTexture.h>

namespace Core
{

Ref<Texture> Texture::Create(const std::string& filepath)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return std::make_shared<OpenGLTexture>(filepath);
	default:
		break;
	}

	assert(false && "Renderer API is not set when creating texture!");
	return nullptr;
}

}
