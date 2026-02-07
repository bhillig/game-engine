#pragma once

#include <Events/Event.h>

namespace Core
{

class Layer
{
public:
	virtual ~Layer() = default;

	virtual void OnUpdate(float deltaTime) {};

	virtual void OnRender() {};

	virtual void OnImGuiRender() {};

	virtual void OnEvent(Event& event) {};

};

} // namespace Core