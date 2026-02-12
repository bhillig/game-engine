#include <Platform/OpenGL/OpenGLElementBuffer.h>

namespace Core
{

OpenGLElementBuffer::OpenGLElementBuffer(const void* data, uint32_t dataSize)
	: OpenGLElementBuffer(data, dataSize, GL_STATIC_DRAW)
{
}

OpenGLElementBuffer::OpenGLElementBuffer(const void* data, uint32_t dataSize, GLenum usage)
{
	// Generate an element buffer
	glGenBuffers(1, &m_rendererID);

	// Bind to the element buffer and pass in our indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, usage);

	// Unbind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Store the element count
	m_count = dataSize / sizeof(unsigned int);
}

OpenGLElementBuffer::~OpenGLElementBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void OpenGLElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void OpenGLElementBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
