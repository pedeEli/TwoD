#pragma once
#include "EventTypes.hpp"
#include "TwoD/Inputs/InputDefines.hpp"

namespace TwoD
{
	class QuitEvent : public EventHandler::Event<EventType>
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::QUIT)
	};

	class ClipboardEvent : public EventHandler::Event<EventType>
	{
	public:
		TD_EVENT_GETTERS(EventType, EventType::CLIPBOARD_UPDATE)

	public:
		bool isOwner;
		int32_t numMimeTypes;
		const char** mineTypes;
	};
}