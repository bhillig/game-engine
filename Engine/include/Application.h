#pragma once

#include <AssetManager.h>
#include <Window.h>
#include <Layer.h>

#include <Events/Event.h>

#include <memory>
#include <cassert>

class Layer;

namespace Core
{
	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
	};

class Application
{
public:
	Application(const ApplicationSpecification& appSpec);
	~Application();

	bool Init();

	static Application* GetApp()
	{
		assert(s_instance && "Application has not been created!");
		return s_instance;
	}

	void Run();

	const Window& GetWindow() const;

	AssetManager& GetAssetManager() { return m_assetManager; }

	template<typename T>
	void AddLayer()
	{
		m_layerStack.emplace_back(std::make_unique<T>());
	}

private:

	void RaiseEvent(Event& event);

private:
	ApplicationSpecification m_appSpec;
	std::unique_ptr<Window> m_window;

	AssetManager m_assetManager;

	float m_deltaTime;
	float m_lastTime;

	std::vector<std::unique_ptr<Layer>> m_layerStack;

	static Application* s_instance;
};

}