#pragma once
#include "Event.hpp"
#include "EventTypes.hpp"
#include "TwoD/Inputs/InputDefines.hpp"

namespace TwoD
{
	class QuitEvent : public Event<EventType>
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::QUIT)
	};

	class ClipboardEvent : public Event<EventType>
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::CLIPBOARD_UPDATE)

	public:
		bool isOwner;
		int32_t numMimeTypes;
		const char** mineTypes;
	};
}