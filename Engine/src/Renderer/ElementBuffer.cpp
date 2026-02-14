#include <Renderer/ElementBuffer.h>

#include <Renderer.h>

#include <Platform/OpenGL/OpenGLElementBuffer.h>

namespace Core
{

std::shared_ptr<ElementBuffer> ElementBuffer::Create(const void* data, uint32_t dataSize)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::OpenGL:
		return std::make_shared<OpenGLElementBuffer>(data, dataSize);
	case RendererAPI::None:
		break;
	}

	assert(false && "Renderer API not valid when creating ElementBuffer!");
	return nullptr;
}

}
