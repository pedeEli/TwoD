#pragma once
#include "EventTypes.hpp"

#define TD_EVENT_GETTERS(type, name) \
	static type GetStaticType() { return name; } \
	type GetEventType() const override { return name; } \
	const char* GetEventName() const override { return #name; }


namespace TwoD
{
	class App;

	class EventHandler
	{
	public:
		struct Handle
		{
		public:
			Handle() = default;

		private:
			Handle(size_t id, EventType type) : m_id(id), m_eventType(type) {}

		private:
			size_t m_id = 0;
			EventType m_eventType = EventType::INVALID;

			friend class EventHandler;
		};

		template<typename EventType>
		class Event
		{
		public:
			Event() = default;
			virtual ~Event() = default;
			Event(const Event& event) = delete;
			Event(Event&& event) = delete;
			Event& operator=(const Event& event) = delete;
			Event& operator=(Event&& event) = delete;

			virtual EventType GetEventType() const = 0;
			virtual const char* GetEventName() const = 0;
		};

		template<class E>
		using Callback = std::function<bool(const E&)>;
		using GenericCallback = Callback<Event<EventType>>;

	public:
		template<class E>
		requires(std::is_base_of_v<Event<EventType>, E>)
		[[nodiscard]] static Handle Add(Callback<E> callback)
		{
			auto& callbacks = m_callbacks[E::GetStaticType()];
			Handle handle = { m_handle++, E::GetStaticType() };
			callbacks.emplace_back(handle, std::move(*(GenericCallback*)&callback));
			return handle;
		}

		static void Remove(Handle handle)
		{
			auto& callbacks = m_callbacks[handle.m_eventType];
			auto it = std::find_if(callbacks.begin(), callbacks.end(), [handle](const auto& pair)
				{
					return pair.first.m_id == handle.m_id;
				});
			if (it != callbacks.end())
			{
				callbacks.erase(it);
			}
		}

	private:
		static void Update();
		static void EmitEvent(const Event<EventType>& event);

	private:
		static inline std::unordered_map<
			EventType,
			std::vector<std::pair<Handle, GenericCallback>>
		> m_callbacks;
		static inline size_t m_handle = 0;

		friend class App;
	};
}

#include "KeyboardEvents.hpp"
#include "MouseEvents.hpp"
#include "MiscEvents.hpp"
#include "WindowEvents.hpp"