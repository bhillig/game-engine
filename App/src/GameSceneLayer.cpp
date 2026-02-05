#include <GameSceneLayer.h>

#include <Application.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Events/InputEvents.h>

#include <Scene/Scene.h>

GameSceneLayer::GameSceneLayer()
	: m_sceneHasFocus(true)
{
	m_scene = std::make_unique<Scene>();
}

void GameSceneLayer::OnUpdate(float deltaTime)
{
	// Simulate the current scene
	m_scene->Simulate(deltaTime);
}

void GameSceneLayer::OnRender()
{
	m_scene->Render();
}

void GameSceneLayer::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::KeyPressedEvent>([this](const Core::KeyPressedEvent& event) { return OnKeyPressed(event.GetKeyCode()); });
	m_scene->OnEvent(event);
}

bool GameSceneLayer::OnKeyPressed(int keyCode)
{
	// Toggle scene focus
	if (keyCode == KEY_ESCAPE)
	{
		ToggleSceneFocus();
	}
	return false;
}

void GameSceneLayer::ToggleSceneFocus()
{
	m_sceneHasFocus = !m_sceneHasFocus;
	if (m_sceneHasFocus)
	{
		m_scene->OnGainFocus();
	}
	else
	{
		m_scene->OnLoseFocus();
	}
}
