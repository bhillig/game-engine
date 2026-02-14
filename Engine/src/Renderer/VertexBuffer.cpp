#include <Renderer/VertexBuffer.h>

#include <Renderer.h>

#include <Platform/OpenGL/OpenGLVertexBuffer.h>

namespace Core
{

std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t dataSize)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::OpenGL:
		return std::make_shared<OpenGLVertexBuffer>(data, dataSize);
	case RendererAPI::None:
		break;
	}

	assert(false && "Renderer API not valid when creating VertexBuffer!");
	return nullptr;
}

}
