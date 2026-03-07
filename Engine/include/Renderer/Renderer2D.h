#pragma once

#include <Renderer/Texture.h>

namespace Core
{
class OrthographicCamera;
}

namespace Core
{

class Renderer2D
{
public:

	static void Init();

	static void BeginScene(const OrthographicCamera& camera);

	static void EndScene();

	static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color);
	static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const glm::vec4& color);
	static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const Ref<Texture>& texture, float tilingFactor = 1.f);
	static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const Ref<Texture>& texture, float tilingFactor = 1.f);

private:
	Renderer2D() = default;
};

}
