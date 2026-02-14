#include <Platform/OpenGL/OpenGLVertexBuffer.h>

#include <glad/glad.h>

namespace Core
{

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t dataSize)
{
	// Generate a vertex buffer
	glGenBuffers(1, &m_rendererID);

	// Bind to the vertex buffer and pass in our vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);

	// Unbind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
