#pragma once

#include <Renderer/VertexBuffer.h>

#include <glad/glad.h>

namespace Core
{

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const void* data, uint32_t dataSize);
	OpenGLVertexBuffer(const void* data, uint32_t dataSize, GLenum usage);
	~OpenGLVertexBuffer() override;

	void Bind() const override;

	void Unbind() const override;

private:
	unsigned int m_rendererID;
};

}
