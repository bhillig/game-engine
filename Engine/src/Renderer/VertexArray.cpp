#include <Renderer/VertexArray.h>

#include <glad/glad.h>

namespace Core
{

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
	if (m_rendererID != 0)
	{
		glDeleteVertexArrays(1, &m_rendererID);
	}
}

VertexArray::VertexArray(VertexArray&& other) noexcept : m_rendererID(other.m_rendererID)
{
	other.m_rendererID = 0; // prevent deletion
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(1, &m_rendererID); // delete current
		m_rendererID = other.m_rendererID;              // steal ID
		other.m_rendererID = 0;
	}
	return *this;
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

}
