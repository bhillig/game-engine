#pragma once

#include <Layer.h>

#include <Scene/Scene.h>
#include <Events/Event.h>

#include <memory>

class GameSceneLayer : public Core::Layer
{

public:
	GameSceneLayer();

	void OnUpdate(float deltaTime) override;

	void OnRender() override;

	void OnEvent(Core::Event& event) override;

// Event callbacks
private:
	bool OnKeyPressed(int keyCode);

private:

	void ToggleSceneFocus();

	bool m_sceneHasFocus;

	std::unique_ptr<Scene> m_scene;
};