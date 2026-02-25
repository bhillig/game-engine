#pragma once

#include <Events/Event.h>

#include <ECS/EntityManager.h>

#include <PerspectiveCamera.h>
#include <PerspectiveCameraController.h>

#include "OrthographicCamera.h"
#include "OrthographicCameraController.h"

class Scene
{
public:
	Scene();

	ECS::EntityManager& GetEntityManager() { return m_entityManager; }

	// Called every timeSteps times per frame to update the scene's logic
	void Update(float deltaTime);

	// Called every frame to render the gui
	void ImGuiRender();

	// Process events
	void OnEvent(Core::Event& event);
	
	// Called when the scene gains focus
	void OnGainFocus();

	// Called when the scene loses focus
	void OnLoseFocus();

private:

	// Event callbacks
	bool OnKeyPressed(int key);
	bool OnKeyReleased(int key);
	bool OnMouseMove(double xPos, double yPos);

	// ImGui Helpers
	void ConstructLevelTreeTab();
	void ConstructWorldTab();
	void ConstructCameraTab();

private:
	ECS::EntityManager m_entityManager; // Contains all the entities for the scene

	Core::OrthographicCameraController m_orthoCameraController; // Camera controller

	Core::PerspectiveCamera m_perspectiveCamera;
	Core::PerspectiveCameraController m_perspectiveCameraController;

	Core::Ref<Core::Texture> m_texture;

	bool m_enable3DView; // Whether 3D view is selected
	bool m_skyBoxEnabled; // Whether skybox is enabled
	bool m_useSpaceSkybox; // Toggle between skybox and space-skybox
};
