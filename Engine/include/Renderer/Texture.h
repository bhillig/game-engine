#pragma once

#include <Core.h>

namespace Core
{

class Texture
{
public:
	virtual ~Texture() = default;

	virtual void Bind(int slot = 0) const = 0;

	virtual void Unbind() const = 0;

	static Ref<Texture> Create(const std::string& filepath);
};

}
