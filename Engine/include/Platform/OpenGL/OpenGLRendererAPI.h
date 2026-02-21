#pragma once

#include <Renderer/RendererAPI.h>

namespace Core
{
	
class OpenGLRendererAPI : public RendererAPI
{
public:
	OpenGLRendererAPI();
	~OpenGLRendererAPI() override;

	void Init() override;

	void SetClearColor(const glm::vec4& color) override;

	void Clear() override;

	void DrawIndexed(const Ref<VertexArray>& vertexArray, DrawMode drawMode) override;

	RendererCapabilities& GetCapabilities() override { return m_rendererCapabilities; }

private:
	RendererCapabilities m_rendererCapabilities;
};

}
