#pragma once

#include <Renderer/Texture.h>

namespace Core
{

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(const std::string& filepath, int slot = 0);
	~OpenGLTexture() override;

	void Bind(int slot = 0) const override;

	void Unbind() const override;

private:
	unsigned int m_rendererID;
	int m_width;
	int m_height;
	int m_numberOfColorChannels;
	unsigned char* m_buffer;
};

}
