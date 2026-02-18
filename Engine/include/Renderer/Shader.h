#pragma once

#include <unordered_map>
#include <string>

class Shader
{
public:
	Shader(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath);
	~Shader();

	void Bind() const;

	void Unbind() const;

	bool SetUniform1f(const char* name, float value);

	bool GetUniform1f(const char* name, float& value);

	bool SetUniform3f(const char* name, float v0, float v1, float v2);

	bool SetUniform4f(const char* name, float v0, float v1, float v2, float v3);

	bool SetUniform1i(const char* name, int value);

	bool SetUniform1b(const char* name, bool value);

	bool SetUniformMatrix4fv(const char* name, const float* value);

private:

	int GetUniformLocation(const std::string& name);

	unsigned int m_shaderProgram;
	std::unordered_map<std::string, int> m_uniformCache;
};