#pragma once

#include <Renderer/RendererAPI.h>
#include <Renderer/VertexArray.h>

namespace Core
{
	
class RenderCommand
{
public:

	static void Init()
	{
		s_rendererAPI->Init();
	}

	static void SetViewport(int x, int y, int width, int height)
	{
		s_rendererAPI->SetViewport(x, y, width, height);
	}

	static RendererAPI::RendererCapabilities& GetCapabilities()
	{
		return s_rendererAPI->GetCapabilities();
	}

	static void SetClearColor(const glm::vec4& color)
	{
		s_rendererAPI->SetClearColor(color);
	}

	static void Clear()
	{
		s_rendererAPI->Clear();
	}

	static void DrawIndexed(const Ref<VertexArray>& vertexArray, RendererAPI::DrawMode drawMode)
	{
		s_rendererAPI->DrawIndexed(vertexArray, drawMode);
	}

	static RendererAPI* s_rendererAPI;
};

}
