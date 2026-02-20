#pragma once

#include <Core.h>

namespace Core
{

class ElementBuffer
{
public:
	virtual ~ElementBuffer() = default;

	virtual void Bind() const = 0;

	virtual void Unbind() const = 0;

	virtual unsigned int Count() const = 0;

	static Ref<ElementBuffer> Create(const void* data, uint32_t dataSize);
};

}
