#pragma once

#include <Core.h>

namespace Core
{

class CubemapTexture
{
public:
	virtual ~CubemapTexture() = default;

	virtual void Bind(int slot = 0) const = 0;

	virtual void Unbind() const = 0 ;

	static constexpr unsigned int NUM_FACES = 6;

	static Ref<CubemapTexture> Create(std::string_view posX, std::string_view negX, std::string_view posY, std::string_view negY, std::string_view posZ, std::string_view negZ, int slot = 0);
};

}
