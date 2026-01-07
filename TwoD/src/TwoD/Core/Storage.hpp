#pragma once
#include <vector>
#include <concepts>
#include <unordered_map>

#include "TwoD/Core/Ref.hpp"
#include "TwoD/Core/Asserts.hpp"

namespace TwoD
{
	template<typename Handle>
	class HasHandle
	{
	public:
		virtual Handle GetHandle() const = 0;
	};

	template<typename T, typename Handle>
	requires(std::is_base_of_v<HasHandle<Handle>, T>)
	class Storage
	{
	public:
		template<typename... Args>
		Ref<T> Add(Handle handle, Args&&... args)
		{
			TD_CORE_ASSERT(!m_indices.contains(handle))
			m_items.emplace_back(std::forward<Args>(args)...);
			auto index = m_items.size() - 1;
			m_indices.emplace(handle, index);
			return Ref<T>(&m_items, index);
		}
		Ref<T> Get(Handle handle)
		{
			TD_CORE_ASSERT(m_indices.contains(handle))
			auto it = m_indices.find(handle);
			return Ref<T>(&m_items, it->second);
		}
		std::vector<T>& GetAll()
		{
			return m_items;
		}
		T Destroy(Handle handle)
		{
			TD_CORE_ASSERT(m_indices.contains(handle))
			
			auto it = m_indices.find(handle);
			auto index = it->second;
			m_indices.erase(it);
			
			if (index == m_items.size() - 1)
			{
				auto item = std::move(m_items.back());
				m_items.pop_back();
				return item;
			}

			auto& back = m_items.back();
			auto item = std::move(m_items[index]);
			m_items[index] = std::move(back);
			m_indices[back.GetHandle()] = index;
			m_items.pop_back();
			return item;
		}

	protected:
		std::vector<T> m_items;
		std::unordered_map<Handle, size_t> m_indices;
	};
}