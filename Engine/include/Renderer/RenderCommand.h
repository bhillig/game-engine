#pragma once

#include <Renderer/RendererAPI.h>
#include <Renderer/VertexArray.h>

namespace Core
{
	
class RenderCommand
{
public:

	// TODO: Figure out a better way for RendererAPI to initialize
	static void Init()
	{
		s_rendererAPI->Init();
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
