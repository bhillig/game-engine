#pragma once

#include <string>
#include <functional>

namespace Core
{

enum class EventType
{
	None = 0,
	MouseMoved, MouseButtonPressed, MouseButtonReleased,
	KeyPressed, KeyReleased,
	WindowClosed, WindowResized
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
class Event
{
public:
	bool Handled = false;
	virtual ~Event() {}
	virtual EventType GetType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); }
};

class EventDispatcher
{
	template<typename T>
	using EventFunction = std::function<bool(T&)>;

public:
	EventDispatcher(Event& event) 
		: m_event(event)
	{
	}

	template<typename T>
	bool Dispatch(EventFunction<T> func)
	{
		if (m_event.GetType() == T::GetStaticType() && !m_event.Handled)
		{
			m_event.Handled = func(*(T*)&m_event);
			return true;
		}
		return false;
	}

private:
	Event& m_event;
};

} // namespace Core