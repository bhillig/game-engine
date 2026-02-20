#pragma once

namespace Core
{

class Texture
{
public:
	virtual ~Texture() = default;

	virtual void Bind(int slot = 0) const = 0;

	virtual void Unbind() const = 0;

	static std::shared_ptr<Texture> Create(const std::string& filepath, int slot = 0);
};

}
