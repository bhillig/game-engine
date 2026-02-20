#pragma once

namespace Core
{

class Shader
{
public:
	virtual ~Shader() = default;

	virtual void Bind() const = 0;

	virtual void Unbind() const = 0;

	virtual bool SetUniform1f(const char* name, float value) = 0;

	virtual bool GetUniform1f(const char* name, float& value) = 0;

	virtual bool SetUniform3f(const char* name, float v0, float v1, float v2) = 0;

	virtual bool SetUniform4f(const char* name, float v0, float v1, float v2, float v3) = 0;

	virtual bool SetUniform1i(const char* name, int value) = 0;

	virtual bool SetUniform1b(const char* name, bool value) = 0;

	virtual bool SetUniformMatrix4fv(const char* name, const float* value) = 0;

	static std::shared_ptr<Shader> Create(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath);
};

}
