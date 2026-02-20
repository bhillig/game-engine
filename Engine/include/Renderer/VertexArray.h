#pragma once

#include <Core.h>

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

	virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;

	virtual void SetElementBuffer(const Ref<ElementBuffer>& elementBuffer) = 0;

	virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;

	virtual Ref<ElementBuffer> GetElementBuffer() const = 0;

	static Ref<VertexArray> Create();
};

}

