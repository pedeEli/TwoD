#pragma once
#include "Event.hpp"
#include "EventTypes.hpp"
#include "TwoD/Inputs/InputDefines.hpp"

namespace TwoD
{
	class WindowEvent : public Event<EventType>
	{
	public:
		uint32_t windowID{};
		int32_t x{};
		int32_t y{};
	};

	class WindowResizedEvent : public WindowEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::WINDOW_RESIZED)
	};

	class WindowCloseRequestedEvent : public WindowEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::WINDOW_CLOSE_REQUESTED)
	};
}