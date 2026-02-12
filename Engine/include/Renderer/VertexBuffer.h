#pragma once

namespace Core
{

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;

	virtual void Bind() const = 0;

	virtual void Unbind() const = 0;

	static VertexBuffer* Create(const void* data, uint32_t dataSize);
};

}
