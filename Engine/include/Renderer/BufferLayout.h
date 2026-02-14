#pragma once

#include <vector>
#include <string>

namespace Core
{

enum class ShaderDataType : uint8_t
{
	None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Double, Double2, Double3, Double4,
	Mat3, Mat4, Bool
};

static uint32_t ShaderDataTypeCount(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::None:
		return 0;
	case ShaderDataType::Float:
	case ShaderDataType::Int:
	case ShaderDataType::Double:
	case ShaderDataType::Bool:
		return 1;
	case ShaderDataType::Float2:
	case ShaderDataType::Int2:
	case ShaderDataType::Double2:
		return 2;
	case ShaderDataType::Float3:
	case ShaderDataType::Int3:
	case ShaderDataType::Double3:
		return 3;
	case ShaderDataType::Float4:
	case ShaderDataType::Int4:
	case ShaderDataType::Double4:
		return 4;
	case ShaderDataType::Mat3:
		return 3 * 3;
	case ShaderDataType::Mat4:
		return 4 * 4;
	}
	return 0;
}
static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::None:
		return 0;
	case ShaderDataType::Float:
		return sizeof(float);
	case ShaderDataType::Float2:
		return sizeof(float) * 2;
	case ShaderDataType::Float3:
		return sizeof(float) * 3;
	case ShaderDataType::Float4:
		return sizeof(float) * 4;
	case ShaderDataType::Int:
		return sizeof(int);
	case ShaderDataType::Int2:
		return sizeof(int) * 2;
	case ShaderDataType::Int3:
		return sizeof(int) * 3;
	case ShaderDataType::Int4:
		return sizeof(int) * 4;
	case ShaderDataType::Double:
		return sizeof(double);
	case ShaderDataType::Double2:
		return sizeof(double) * 2;
	case ShaderDataType::Double3:
		return sizeof(double) * 3;
	case ShaderDataType::Double4:
		return sizeof(double) * 4;
	case ShaderDataType::Mat3:
		return sizeof(float) * 3 * 3;
	case ShaderDataType::Mat4:
		return sizeof(float) * 4 * 4;
	case ShaderDataType::Bool:
		return sizeof(bool);
	}
	return 0;
}

struct BufferElement
{
	ShaderDataType Type;
	std::string Name;
	uint32_t Size;
	uint32_t Count;
	uint32_t Offset;

	BufferElement(ShaderDataType type, std::string name)
		: Type(type), Name(std::move(name)), Size(ShaderDataTypeSize(type)), Count(ShaderDataTypeCount(type)), Offset(0)
	{
	}
};

class BufferLayout
{
public:
	BufferLayout() = default;
	BufferLayout(std::vector<BufferElement> elements)
		: m_elements(std::move(elements)), m_stride(0)
	{
		for (auto& element : m_elements)
		{
			element.Offset = m_stride;
			m_stride += element.Size;
		}
	}

	std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	const std::vector<BufferElement>& GetElements() const { return m_elements; }

	uint32_t GetStride() const { return m_stride; }

private:
	std::vector<BufferElement> m_elements;
	uint32_t m_stride;
};

}
