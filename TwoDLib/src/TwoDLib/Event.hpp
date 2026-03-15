#pragma once
#include <functional>
#include <vector>
#include <algorithm>

namespace TwoD
{
	template<typename... Args>
	class Event
	{
	public:
		using Callback = std::function<void(const Args&... args)>;
		struct Handle
		{
		private:
			size_t m_id = 0;

			friend class Event<Args...>;
		};

	public:
		Handle Add(Callback callback)
		{
			Handle handle = { m_handle++ };
			m_callbacks.emplace_back(handle, std::move(callback));
			return handle;
		}
		void Remove(Handle handle)
		{
			auto it = std::find_if(m_callbacks.begin(), m_callbacks.end(), [handle](const auto& pair)
				{
					return pair.first.m_id == handle.m_id;
				});
			if (it != m_callbacks.end())
			{
				m_callbacks.erase(it);
			}
		}

		void Emit(const Args&... args) const
		{
			for (auto& callback : m_callbacks)
			{
				callback(args...);
			}
		}
	private:
		std::vector<std::pair<Handle, Callback>> m_callbacks;
		size_t m_handle = 0;
	};
}