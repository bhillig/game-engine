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

	enum class DrawMode
	{
		None = 0,
		Lines = 1,
		Triangles = 2
	};

	struct RendererCapabilities
	{
		std::string Vendor;
		std::string Renderer;
		std::string Version;
	};

public:
	virtual ~RendererAPI() = default;

	virtual void Init() = 0;

	virtual void SetViewport(int x, int y, int width, int height) = 0;

	virtual void SetClearColor(const glm::vec4& color) = 0;

	virtual void Clear() = 0;

	virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, DrawMode drawMode) = 0;

	// Sets the Renderer API (OpenGL, Vulkan, etc.)
	// @param api - The API to use
	static void SetAPI(API api) { s_API = api; }

	// Returns the Renderer API (OpenGL, Vulkan, etc.)
	static API GetAPI() { return s_API; }

	virtual RendererCapabilities& GetCapabilities() = 0;

private:
	static API s_API;
};

}
