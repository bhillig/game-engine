#pragma once

#include <Events/Event.h>

namespace Core
{

//
// Key Events
//
class KeyEvent : public Event
{
public:
	inline int GetKeyCode() const { return m_keyCode; }

protected:
	KeyEvent(int keyCode)
		: m_keyCode(keyCode) {}

	int m_keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keyCode, bool isRepeated)
		: KeyEvent(keyCode), m_repeated(isRepeated) {}

	inline bool IsRepeated() const { return m_repeated; }

	EVENT_CLASS_TYPE(KeyPressed);

private:
	bool m_repeated;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keyCode)
		: KeyEvent(keyCode) {}

	EVENT_CLASS_TYPE(KeyReleased);
};

//
// Mouse Events
//
class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(double x, double y)
		: m_mouseX(x), m_mouseY(y) {}

	inline double GetX() const { return m_mouseX; }

	inline double GetY() const { return m_mouseY; }

	EVENT_CLASS_TYPE(MouseMoved);

private:
	double m_mouseX, m_mouseY;
};

class MouseButtonPressedEvent : public Event
{
public:
	MouseButtonPressedEvent(int button)
		: m_button(button) {}

	inline int GetButton() const { return m_button; }

	EVENT_CLASS_TYPE(MouseButtonPressed);

private:
	int m_button;
};

class MouseButtonReleasedEvent : public Event
{
public:
	MouseButtonReleasedEvent(int button)
		: m_button(button) {
	}

	inline int GetButton() const { return m_button; }

	EVENT_CLASS_TYPE(MouseButtonReleased);

private:
	int m_button;
};

} // namespace Core