#include <Platform/OpenGL/OpenGLVertexArray.h>

#include <Renderer/BufferLayout.h>

#include <glad/glad.h>

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

OpenGLVertexArray::OpenGLVertexArray()
{
	glGenVertexArrays(1, &m_rendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	const auto& layout = vertexBuffer->GetLayout();
	assert(!layout.GetElements().empty() && "VertexBufferLayout is empty!");

	glBindVertexArray(m_rendererID);
	vertexBuffer->Bind();
	int index = 0;
	for (const auto& element : layout)
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
	m_vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer)
{
	glBindVertexArray(m_rendererID);
	elementBuffer->Bind();

	m_elementBuffer = elementBuffer;
}

}
