#pragma once

#include <Renderer/VertexBuffer.h>
#include <Renderer/BufferLayout.h>

namespace Core
{

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const void* data, uint32_t dataSize);
	~OpenGLVertexBuffer() override;

	void Bind() const override;

	void Unbind() const override;

	void SetLayout(const BufferLayout& layout) override { m_layout = layout; }

	const BufferLayout& GetLayout() const override { return m_layout; }

private:
	unsigned int m_rendererID;
	BufferLayout m_layout;
};

}
