#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class CubemapTexture;
class Model;
class Shader;

namespace Core
{
	
class Renderer
{
public:
	~Renderer();

	// Sets the view and projection matrices for rendering
	static void BeginScene(const glm::mat4& view, const glm::mat4& projection);

	// Clears the view and projection matrices for rendering
	static void EndScene();

	// Sets the renderer's clear color
	// @param color - The value to set the color buffer
	static void ClearColor(const glm::vec4& color);

	// Clears the color and depth buffer. Typically called at the beginning of a new frame
	static void ClearScreen();

	// Draws a bounding box defined by (min,max) where min is the bottom-left corner and max is the top-right corner.
	// @param min - Bottom-left corner of the bounding box to be rendered
	// @param max - Top-right corner of the bounding box to be rendered
	static void DrawBoundingBox(const glm::vec3& min, const glm::vec3& max);

	// Draws a line between two vertices. Positions should be in world-space
	// @param a - First vertex
	// @param b - Second vertex
	// @param color - Color of the line to be drawn
	static void DrawLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color = glm::vec3(1.f));

	// Draws the engine's skybox
	static void DrawSkybox();

	// Draws the engine's space skybox
	static void DrawSpaceSkybox();

	// TODO: Implement a function which allows users to pass in a skybox texture so games can apply their own skybox's

	// Submits a model to be rendered
	// @param model - The model to render
	// @param transform - The world transform of the model
	static void Submit(const Model& model, const glm::mat4& transform);

private:

	// Called by Application to initialize
	bool Initialize();

	// Implementation of BeginScene
	void BeginSceneImpl(const glm::mat4& view, const glm::mat4& projection);

	// Implementation of EndScene
	void EndSceneImpl();

	// Implementation of ClearColor
	void ClearColorImpl(const glm::vec4& color);

	// Implementation of ClearScreen
	void ClearScreenImpl();

	// Implementation of DrawBoundingBox
	void DrawBoundingBoxImpl(const glm::vec3& min, const glm::vec3& max);

	// Implementation of DrawLine
	void DrawLineImpl(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color = glm::vec3(1.f));

	// Implementation of DrawSkybox
	void DrawSkyboxImpl();

	// Implementation of DrawSpaceSkybox
	void DrawSpaceSkyboxImpl();

	// Implementation of Submit
	void SubmitImpl(const Model& model, const glm::mat4& transform);

	friend class Application;

	Renderer();
	static Renderer& GetRenderer(); // Returns Renderer instance from Application

	glm::mat4 m_view;
	glm::mat4 m_projection;

	std::unique_ptr<Shader> m_modelShader;
	std::unique_ptr<Shader> m_lineShader;
	std::unique_ptr<Shader> m_cubeMapShader;

	std::unique_ptr<CubemapTexture> m_skyBoxTexture;
	std::unique_ptr<CubemapTexture> m_spaceSkyBoxTexture;
};

}
