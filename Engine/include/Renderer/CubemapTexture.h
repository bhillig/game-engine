#pragma once

#include <array>
#include <string>
#include <string_view>

#include <glad/glad.h>

class CubemapTexture
{
public:
	CubemapTexture(std::string_view posX, std::string_view negX, std::string_view posY, std::string_view negY, std::string_view posZ, std::string_view negZ, int slot = 0);
	~CubemapTexture();

	void Bind(int slot = 0) const;

	void Unbind() const;

	static constexpr unsigned int NUM_FACES = 6;

private:
	unsigned int m_rendererID;
	std::array<std::string, NUM_FACES> m_filePaths;
};
