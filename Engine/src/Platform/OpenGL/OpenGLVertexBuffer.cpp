#include <Platform/OpenGL/OpenGLVertexBuffer.h>

#include <Renderer/BufferLayout.h>

namespace
{

unsigned int ShaderDataTypeToOpenGLType(Core::ShaderDataType type)
{
	switch (type)
	{
	case Core::ShaderDataType::Float:
	case Core::ShaderDataType::Float2:
	case Core::ShaderDataType::Float3:
	case Core::ShaderDataType::Float4:
	case Core::ShaderDataType::Mat3:
	case Core::ShaderDataType::Mat4:
		return GL_FLOAT;
	case Core::ShaderDataType::Double:
	case Core::ShaderDataType::Double2:
	case Core::ShaderDataType::Double3:
	case Core::ShaderDataType::Double4:
		return GL_DOUBLE;
	case Core::ShaderDataType::Int:
	case Core::ShaderDataType::Int2:
	case Core::ShaderDataType::Int3:
	case Core::ShaderDataType::Int4:
		return GL_INT;
	case Core::ShaderDataType::Bool:
		return GL_BOOL;
	case Core::ShaderDataType::None:
		assert(false && "ShaderDataType is None!");
		return 0;
	}
	return 0;
}

}

namespace Core
{

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t dataSize)
: OpenGLVertexBuffer(data, dataSize, GL_STATIC_DRAW)
{
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t dataSize, GLenum usage)
{
	// Generate a vertex buffer
	glGenBuffers(1, &m_rendererID);

	// Bind to the vertex buffer and pass in our vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);

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

void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) const
{
	Bind();
	const std::vector<BufferElement>& elements = layout.GetElements();
	int index = 0;
	for (const auto& element : elements)
	{
		glVertexAttribPointer(index, 
			static_cast<int>(element.Count), 
			ShaderDataTypeToOpenGLType(element.Type), 
			GL_FALSE, 
			static_cast<int>(layout.GetStride()), 
			(void*)element.Offset);
		glEnableVertexAttribArray(index);
		index++;
	}
}

}
