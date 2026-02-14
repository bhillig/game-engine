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

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;

	void SetElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer) override;

	const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; }

	std::shared_ptr<ElementBuffer> GetElementBuffer() const override { return m_elementBuffer; }

private:
	uint32_t m_rendererID;
	std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
	std::shared_ptr<ElementBuffer> m_elementBuffer;
};

}
