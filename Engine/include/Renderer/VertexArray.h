#pragma once

#include <Renderer/ElementBuffer.h>
#include <Renderer/VertexBuffer.h>

namespace Core
{

class VertexArray
{
public:
	virtual ~VertexArray() = default;

	virtual void Bind() const = 0;

	virtual void Unbind() const = 0;

	virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;

	virtual void SetElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer) = 0;

	virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;

	virtual std::shared_ptr<ElementBuffer> GetElementBuffer() const = 0;

	static std::shared_ptr<VertexArray> Create();
};

}

