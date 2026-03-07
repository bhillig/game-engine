#include <Platform/OpenGL/OpenGLTexture.h>

#include <stb_image.h>

#include <glad/glad.h>

namespace Core
{

OpenGLTexture::OpenGLTexture(const std::string& filepath)
{
	// Create and bind texture
	glGenTextures(1, &m_rendererID);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture and generate mipmap
	m_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_numberOfColorChannels, 4);
	if (m_buffer)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(m_buffer);
	}
	else
	{
		LOG_CORE_ERROR("Texture failed!");
	}
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &m_rendererID);
}

void OpenGLTexture::Bind(int slot) const
{
	glBindTextureUnit(slot, m_rendererID);
}

void OpenGLTexture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
