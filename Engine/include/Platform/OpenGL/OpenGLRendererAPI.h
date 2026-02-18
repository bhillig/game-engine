#pragma once

#include <Renderer/RendererAPI.h>

namespace Core
{
	
class OpenGLRendererAPI : public RendererAPI
{
public:
	OpenGLRendererAPI();

	~OpenGLRendererAPI() override;

	void SetClearColor(const glm::vec4& color) override;

	void Clear() override;

	void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, DrawMode drawMode) override;

};

}
