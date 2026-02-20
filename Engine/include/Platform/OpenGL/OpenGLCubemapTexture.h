#pragma once

#include <Renderer/CubemapTexture.h>

#include <array>
#include <string>
#include <string_view>

namespace Core
{

class OpenGLCubemapTexture final : public CubemapTexture
{
public:
	OpenGLCubemapTexture(std::string_view posX, std::string_view negX, std::string_view posY, std::string_view negY, std::string_view posZ, std::string_view negZ, int slot = 0);
	~OpenGLCubemapTexture() override;

	void Bind(int slot = 0) const override;

	void Unbind() const override;

private:
	unsigned int m_rendererID;
	std::array<std::string, NUM_FACES> m_filePaths;
};

}
