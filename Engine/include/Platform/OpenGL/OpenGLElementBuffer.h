#pragma once

#include <Renderer/ElementBuffer.h>

#include <glad/glad.h>

namespace Core
{

class OpenGLElementBuffer final : public ElementBuffer
{
public:
	OpenGLElementBuffer(const void* data, uint32_t dataSize);
	OpenGLElementBuffer(const void* data, uint32_t dataSize, GLenum usage);
	~OpenGLElementBuffer() override;

	void Bind() const override;

	void Unbind() const override;

	unsigned int Count() const override { return m_count; }

private:
	unsigned int m_rendererID;
	unsigned int m_count;
};

}
