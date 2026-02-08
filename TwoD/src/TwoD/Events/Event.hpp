#pragma once

#define TD_EVENT_GETTERS(type, name) static type GetStaticType() { return name; } \
type GetEventType() const override { return name; } \
const char* GetEventName() const override { return #name; }

namespace TwoD
{
	template<typename EventType>
	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetEventName() const = 0;
	};
}
