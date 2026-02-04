#pragma once

namespace Core
{

class Input
{
public:

	static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); }

	static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); }

	static glm::vec2 GetMousePosition() { return s_instance->GetMousePositionImpl(); }

protected:

	virtual bool IsKeyPressedImpl(int keycode) = 0;

	virtual bool IsMouseButtonPressedImpl(int button) = 0;

	virtual glm::vec2 GetMousePositionImpl() = 0;

protected:
	static Input* s_instance;
};

}
