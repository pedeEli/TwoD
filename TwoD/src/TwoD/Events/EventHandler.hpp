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
		template<class E>
		requires(std::is_base_of_v<Event<EventType>, E>)
		static void On(Callback<E> callback)
		{
			auto it = m_callbacks.find(E::GetStaticType());
			if (it == m_callbacks.end())
			{
				m_callbacks[E::GetStaticType()] = { *(GenericCallback*)&callback };
				return;
			}
			it->second.push_back(*(GenericCallback*)&callback);
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