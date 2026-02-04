#pragma once

#include <Events/Event.h>

#include <ECS/EntityManager.h>

#include <Camera.h>
#include <CameraController.h>

class Scene
{
public:
	Scene();

	Camera* GetCamera() const { return m_camera.get(); }

	ECS::EntityManager& GetEntityManager() { return m_entityManager; }

	// Simulates the scene
	void Simulate(float deltaTime, unsigned int timeSteps = 1);

	// Called every frame to render the scene
	void Render();

	// Process events
	void OnEvent(Core::Event& event);
	
	// Called when the scene gains focus
	void OnGainFocus();

	// Called when the scene loses focus
	void OnLoseFocus();

protected:

	// Called every timeSteps times per frame to update the scene's logic
	void Update(float deltaTime);

protected:

	// Event callbacks
	bool OnKeyPressed(int key);
	bool OnKeyReleased(int key);
	bool OnMouseMove(double xPos, double yPos);

private:

	// Constructs the scene's GUI
	void ConstructGUI();

	void ConstructLevelTreeTab();
	void ConstructWorldTab();
	void ConstructCameraTab();

protected:
	ECS::EntityManager m_entityManager; // Contains all the entities for the scene
	std::unique_ptr<Camera> m_camera; // Camera for the scene
	std::unique_ptr<CameraController> m_cameraController; // CameraController for passing input to the camera

private:
	bool m_skyBoxEnabled; // Whether skybox is enabled
	bool m_useSpaceSkybox; // Toggle between skybox and space-skybox
};