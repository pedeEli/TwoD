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
			void Off()
			{
				auto& callbacks = m_callbacks[eventType];
				auto& back = callbacks.back();
				callbacks[index] = std::move(back);
				callbacks.pop_back();
			}

			size_t index = 0;
			EventType eventType = EventType::INVALID;
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
		[[nodiscard]] static Handle On(Callback<E> callback)
		{
			auto& callbacks = m_callbacks[E::GetStaticType()];
			auto index = callbacks.size();
			callbacks.push_back(*(GenericCallback*)&callback);
			return { index, E::GetStaticType() };
		}

	private:
		static void Update();
		static void EmitEvent(const Event<EventType>& event);

	private:
		static inline std::unordered_map<
			EventType,
			std::vector<GenericCallback>
		> m_callbacks;

		friend class App;
	};
}

#include "KeyboardEvents.hpp"
#include "MouseEvents.hpp"
#include "MiscEvents.hpp"
#include "WindowEvents.hpp"