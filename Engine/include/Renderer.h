#pragma once

#include <Core.h>

#include <Renderer/RendererAPI.h>
#include <Renderer/Shader.h>


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Core
{
	class Texture;
	class Model;


class Renderer
{
public:
	struct SceneData
	{
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
	};
public:
	~Renderer();

	// Sets the scene data for rendering
	static void BeginScene(const SceneData& sceneData);

	// Clears the scene data for rendering
	static void EndScene();

	static ShaderLibrary& GetShaderLibrary();

	// Draws a bounding box defined by (min,max) where min is the bottom-left corner and max is the top-right corner.
	// @param min - Bottom-left corner of the bounding box to be rendered
	// @param max - Top-right corner of the bounding box to be rendered
	static void DrawBoundingBox(const glm::mat4& transform, const glm::vec3& min, const glm::vec3& max);

	// Draws a line between two vertices. Positions should be in world-space
	// @param a - First vertex
	// @param b - Second vertex
	// @param color - Color of the line to be drawn
	static void DrawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color = glm::vec3(1.f));

	// Draws a colored billboard with a given size. Positions should be in world-space
	// @param position - World position
	// @param size - Size of billboard
	// @param color - Color of the billboard to be drawn
	static void DrawBillboard(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	// Draws a textured billboard with a given size. Positions should be in world-space
	// @param position - World position
	// @param size - Size of billboard
	// @param texture - Texture of the billboard to be drawn
	static void DrawBillboard(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture);

	// Draws the engine's skybox
	static void DrawSkybox();

	// Submits a model to be rendered
	// @param model - The model to render
	// @param transform - The world transform of the model
	static void Submit(const Model& model, const glm::mat4& transform);

	static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, RendererAPI::DrawMode drawMode = RendererAPI::DrawMode::Triangles, const glm::mat4& transform = glm::mat4(1.f));

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	static void SetAPI(RendererAPI::API api) { RendererAPI::SetAPI(api); }

private:

	// Called by Application to initialize
	bool Initialize();

	// Implementation of BeginScene
	void BeginSceneImpl(const SceneData& sceneData);

	// Implementation of EndScene
	void EndSceneImpl();

	ShaderLibrary& GetShaderLibraryImpl() { return m_shaderLibrary; }

	// Implementation of DrawBoundingBox
	void DrawBoundingBoxImpl(const glm::mat4& transform, const glm::vec3& min, const glm::vec3& max);

	// Implementation of DrawLine
	void DrawLineImpl(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color = glm::vec3(1.f));

	// Implementation of DrawBillboard
	void DrawBillboardImpl(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	// Implementation of DrawBillboard
	void DrawBillboardImpl(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture);

	// Implementation of DrawSkybox
	void DrawSkyboxImpl();

	// Implementation of Submit (model & transform)
	void SubmitImpl(const Model& model, const glm::mat4& transform);

	// Implementation of Submit (shader, va, and draw mode)
	void SubmitImpl(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, RendererAPI::DrawMode drawMode, const glm::mat4& transform);

	friend class Application;

	Renderer();
	static Renderer& GetRenderer(); // Returns Renderer instance from Application

	SceneData m_sceneData;

	ShaderLibrary m_shaderLibrary;
};

}
