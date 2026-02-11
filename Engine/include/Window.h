#pragma once

#include <Events/Event.h>

#include <string>
#include <functional>

namespace Core
{

struct WindowData
{
	std::string Title;
	int Width = 1920;
	int Height = 1080;
	std::function<void(Event&)> Callback;
};

class Window
{
public:
	virtual ~Window() = default;

	virtual bool Initialize() = 0;

	virtual void Update() = 0;

	virtual int GetWidth() const = 0;

	virtual int GetHeight() const = 0;

	virtual void* GetNativeHandle() const = 0;
};

} // namespace Core