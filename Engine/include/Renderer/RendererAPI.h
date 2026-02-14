#pragma once

#include <Renderer/VertexArray.h>

namespace Core
{

class RendererAPI
{
public:
	enum class API
	{
		None = 0,
		OpenGL = 1
	};
public:
	virtual ~RendererAPI() = default;

	virtual void SetClearColor(const glm::vec4& color) = 0;

	virtual void Clear() = 0;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

	// Sets the Renderer API (OpenGL, Vulkan, etc.)
	// @param api - The API to use
	static void SetAPI(API api) { s_API = api; }

	// Returns the Renderer API (OpenGL, Vulkan, etc.)
	static API GetAPI() { return s_API; }

private:
	static API s_API;
};

}
