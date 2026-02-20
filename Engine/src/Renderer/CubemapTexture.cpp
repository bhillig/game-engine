#include <Renderer/CubemapTexture.h>

#include <Renderer.h>

#include <Platform/OpenGL/OpenGLCubemapTexture.h>

namespace Core
{

Ref<CubemapTexture> CubemapTexture::Create(std::string_view posX, std::string_view negX,
	std::string_view posY, std::string_view negY, std::string_view posZ, std::string_view negZ, int slot)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return std::make_shared<OpenGLCubemapTexture>(posX, negX, posY, negY, posZ, negZ, slot);
	default:
		break;
	}

	assert(false && "Renderer API is not set when creating CubemapTexture!");
	return nullptr;
}

}
