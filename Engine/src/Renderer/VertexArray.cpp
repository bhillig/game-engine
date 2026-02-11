#include <Renderer/VertexArray.h>

#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <glad/glad.h>

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


void VertexArray::Add(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
	Bind();
	buffer.Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	size_t offset = 0;
	for (int i = 0; i < elements.size(); ++i)
	{
		const VertexBufferElement& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
		glEnableVertexAttribArray(i);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
