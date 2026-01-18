#include <iostream>
#include <Renderer/CubemapTexture.h>

#include <stb_image.h>

CubemapTexture::CubemapTexture(std::string_view posX, std::string_view negX, std::string_view posY, std::string_view negY, std::string_view posZ, std::string_view negZ, int slot)
{
	// Cache filepaths to cube faces
	m_filePaths.at(0) = posX;
	m_filePaths.at(1) = negX;
	m_filePaths.at(2) = posY;
	m_filePaths.at(3) = negY;
	m_filePaths.at(4) = posZ;
	m_filePaths.at(5) = negZ;

	// Create and bind cube map texture 
	glGenTextures(1, &m_rendererID);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);

	// Disable flipping textures for cubemap textures
	stbi_set_flip_vertically_on_load(false);

	int width, height, numberOfColorChannels;
	unsigned char* buffer;
	// Load all six texture faces
	for (size_t i = 0; i < NUM_FACES; ++i)
	{
		// Load texture and generate mipmap
		buffer = stbi_load(m_filePaths.at(i).c_str(), &width, &height, &numberOfColorChannels, 0);
		if (buffer)
		{
			const int format = numberOfColorChannels == 4 ? GL_RGBA : GL_RGB;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);
		}
		else
		{
			std::cerr << "Cubemap texture failed to load at path: " << m_filePaths.at(i).c_str() << "\n";
		}
		stbi_image_free(buffer);
	}

	// Re-enable flipping textures
	stbi_set_flip_vertically_on_load(true);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

CubemapTexture::~CubemapTexture()
{
	glDeleteTextures(1, &m_rendererID);
}

void CubemapTexture::Bind(int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);
}

void CubemapTexture::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
