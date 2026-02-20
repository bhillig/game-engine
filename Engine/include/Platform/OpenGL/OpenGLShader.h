#pragma once

#include <Renderer/Shader.h>

#include <unordered_map>
#include <string>

namespace Core
{

class OpenGLShader final : public Shader
{
public:
	OpenGLShader(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath);

	~OpenGLShader() override;

	void Bind() const override;

	void Unbind() const override;

	bool SetUniform1f(const char* name, float value) override;

	bool GetUniform1f(const char* name, float& value) override;

	bool SetUniform3f(const char* name, float v0, float v1, float v2) override;

	bool SetUniform4f(const char* name, float v0, float v1, float v2, float v3) override;

	bool SetUniform1i(const char* name, int value) override;

	bool SetUniform1b(const char* name, bool value) override;

	bool SetUniformMatrix4fv(const char* name, const float* value) override;

private:

	int GetUniformLocation(const std::string& name);

private:

	uint32_t m_shaderProgram;
	std::unordered_map<std::string, int> m_uniformCache;
};

}

