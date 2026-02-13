#pragma once

namespace Core
{

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void Bind() const;

	void Unbind() const;

private:
	unsigned int m_rendererID;
};

}
