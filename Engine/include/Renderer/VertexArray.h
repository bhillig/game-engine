#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

namespace Core
{

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void Add(const VertexBuffer& buffer, const VertexBufferLayout& layout);

	void Bind() const;

	void Unbind() const;

private:
	unsigned int m_rendererID;
};

}