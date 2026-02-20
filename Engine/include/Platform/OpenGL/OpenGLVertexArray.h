#pragma once

#include <Renderer/VertexArray.h>

namespace Core
{

class OpenGLVertexArray final : public VertexArray
{
public:
	OpenGLVertexArray();
	~OpenGLVertexArray() override;

	void Bind() const override;

	void Unbind() const override;

	void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;

	void SetElementBuffer(const Ref<ElementBuffer>& elementBuffer) override;

	const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; }

	Ref<ElementBuffer> GetElementBuffer() const override { return m_elementBuffer; }

private:
	uint32_t m_rendererID;
	std::vector<Ref<VertexBuffer>> m_vertexBuffers;
	Ref<ElementBuffer> m_elementBuffer;
};

}
