#include <Renderer/VertexBuffer.h>

#include <Renderer.h>

#include <Platform/OpenGL/OpenGLVertexBuffer.h>

namespace Core
{

VertexBuffer* VertexBuffer::Create(const void* data, uint32_t dataSize)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::OpenGL:
		return new OpenGLVertexBuffer(data, dataSize);
	case RendererAPI::None:
		break;
	}

	assert(false && "Renderer API not valid when creating VertexBuffer!");
	return nullptr;
}

}
