#pragma once

#include <Input.h>

namespace Core
{

class WindowsInput : public Input
{
protected:

	bool IsKeyPressedImpl(int keycode) override;

	bool IsMouseButtonPressedImpl(int button) override;

	glm::vec2 GetMousePositionImpl() override;

};

}