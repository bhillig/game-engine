#pragma once

#include <Renderer/RendererAPI.h>
#include <Renderer/VertexArray.h>

namespace Core
{
	
class RenderCommand
{
public:

	static void SetClearColor(const glm::vec4& color)
	{
		s_rendererAPI->SetClearColor(color);
	}

	static void Clear()
	{
		s_rendererAPI->Clear();
	}

	static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, RendererAPI::DrawMode drawMode)
	{
		s_rendererAPI->DrawIndexed(vertexArray, drawMode);
	}

	static RendererAPI* s_rendererAPI;
};

}
