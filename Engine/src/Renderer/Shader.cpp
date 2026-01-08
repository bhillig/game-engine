#include <Renderer/Shader.h>

#include <Utils/FileUtils.h>

#include <glad/glad.h>

#include <iostream>

unsigned int CompileShader(const char* shaderSource, int shaderType)
{
	int  success;
	char infoLog[512];

	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);

	// Compile the shader
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Report shader compilation errors
		const std::string shaderTypeStr = shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderTypeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

void LinkShaderProgram(unsigned int shaderProgram)
{
	int  success;
	char infoLog[512];

	// Link the shader program
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::FAILED\n" << infoLog << std::endl;
	}
}

Shader::Shader(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath)
{
	std::string vertexShaderSource;
	fileUtils::ReadTextFromFile(vertexShaderFilePath.data(), vertexShaderSource);
	std::string fragmentShaderSource;
	fileUtils::ReadTextFromFile(fragmentShaderFilePath.data(), fragmentShaderSource);

	unsigned int vertexShader = CompileShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);

	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);

	LinkShaderProgram(m_shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderProgram);
}

void Shader::Bind() const
{
	glUseProgram(m_shaderProgram);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

bool Shader::SetUniform1f(const char* name, float value)
{
	const int uniformLocation = GetUniformLocation(name);
	if (uniformLocation == -1) {
		return false;
	}

	glUseProgram(m_shaderProgram);
	glUniform1f(uniformLocation, value);
	return true;
}

bool Shader::GetUniform1f(const char* name, float& value)
{
	const int uniformLocation = GetUniformLocation(name);
	if (uniformLocation == -1) {
		return false;
	}

	glGetUniformfv(m_shaderProgram, uniformLocation, &value);
	return true;
}

bool Shader::SetUniform3f(const char* name, float v0, float v1, float v2)
{
	const int uniformLocation = GetUniformLocation(name);
	if (uniformLocation == -1) {
		return false;
	}

	glUseProgram(m_shaderProgram);
	glUniform3f(uniformLocation, v0, v1, v2);
	return true;
}

bool Shader::SetUniform4f(const char* name, float v0, float v1, float v2, float v3)
{
	const int uniformLocation = GetUniformLocation(name);
	if (uniformLocation == -1) {
		return false;
	}

	glUseProgram(m_shaderProgram);
	glUniform4f(uniformLocation, v0, v1, v2, v3);
	return true;
}

bool Shader::SetUniform1i(const char* name, int value)
{
	const int uniformLocation = GetUniformLocation(name);
	if (uniformLocation == -1) {
		return false;
	}

	glUseProgram(m_shaderProgram);
	glUniform1i(uniformLocation, value);
	return true;
}

bool Shader::SetUniform1b(const char* name, bool value)
{
	// Bools are the same as ints in OpenGL
	return SetUniform1i(name, value);
}

bool Shader::SetUniformMatrix4fv(const char* name, float* value)
{
	const int uniformLocation = GetUniformLocation(name);
	if (uniformLocation == -1) {
		return false;
	}

	glUseProgram(m_shaderProgram);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value);
	return true;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_uniformCache.contains(name))
	{
		return m_uniformCache.at(name);
	}

	const int uniformLocation = glGetUniformLocation(m_shaderProgram, name.c_str());
	if (uniformLocation != -1)
	{
		// Cache the uniform location
		m_uniformCache[name] = uniformLocation;
	}

	return uniformLocation;
}
