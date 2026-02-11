#pragma once

#include <Window.h>

#include <Renderer/GraphicsContext.h>

struct GLFWwindow;

namespace Core
{

class WindowsWindow final : public Window
{
public:
	WindowsWindow(const WindowData& windowData = WindowData());

	bool Initialize() override;

	void Update() override;

	int GetWidth() const override { return m_windowData.Width; }

	int GetHeight() const override { return m_windowData.Height; }

	void* GetNativeHandle() const override;

protected:
	WindowData m_windowData;
	GLFWwindow* m_window;
	std::unique_ptr<GraphicsContext> m_graphicsContext;
};

}