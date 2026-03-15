#pragma once
#include "EventTypes.hpp"
#include "TwoD/Inputs/InputDefines.hpp"

namespace TwoD
{
	class MouseEvent : public EventHandler::Event<EventType>
	{
	public:
		uint32_t windowID{};
		uint32_t mouseID{};
		float x{};
		float y{};
	};

	class MouseDownEvent : public MouseEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::MOUSE_BUTTON_DOWN)

	public:
		MouseButton button{};
		uint8_t clicks{};
		bool down{};
	};

	class MouseUpEvent : public MouseEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::MOUSE_BUTTON_UP)

	public:
		MouseButton button{};
		uint8_t clicks{};
		bool down{};
	};

	class MouseMotionEvent : public MouseEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::MOUSE_MOTION)

	public:
		MouseButton mouseState{};
		float xrel{};
		float yrel{};
	};

	class MouseWheelEvent : public MouseEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::MOUSE_WHEEL)

	public:
		MouseWheelDirection direction{};
		float mouseX{};
		float mouseY{};
		int32_t scrollTicksX{};
		int32_t scrollTicksY{};
	};
}