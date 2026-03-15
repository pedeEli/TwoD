#pragma once
#include "Event.hpp"
#include "EventTypes.hpp"
#include "KeyboardEvents.hpp"
#include "MouseEvents.hpp"
#include "MiscEvents.hpp"
#include "WindowEvents.hpp"

namespace TwoD
{
	class App;

	class EventHandler
	{
		template<class E>
		using Callback = std::function<bool(const E&)>;
		using GenericCallback = Callback<Event<EventType>>;
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