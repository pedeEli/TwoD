#pragma once
#include "Event.hpp"
#include "EventTypes.hpp"
#include "TwoD/Inputs/InputDefines.hpp"

namespace TwoD
{
	class KeyboardEvent : public Event<EventType>
	{
	public:
		uint32_t windowID;
		uint32_t keyboardID;
		Scancode scancode;
		Key key;
		Keymod mod;
		bool down;
	};

	class KeyDownEvent : public KeyboardEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::KEY_DOWN)
	
	public:
		bool repeat;
	};

	class KeyUpEvent : public KeyboardEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::KEY_UP)
	};


	class TextEvent : public Event<EventType>
	{
	public:
		uint32_t windowID;
		const char* text;
	};

	class TextInputEvent : public TextEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::TEXT_INPUT)
	};

	class TextEditingEvent : public TextEvent
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::TEXT_EDITING)

	public:
		int32_t start;
		int32_t length;
	};
}