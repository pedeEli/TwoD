#pragma once
#include <vector>
#include <concepts>
#include <unordered_map>

#include "TwoD/Core/Asserts.hpp"

namespace TwoD
{
	struct StorageProbe
	{
		template<class T>
		static auto handle(T& t) -> decltype((t.m_storageHandle))
		{
			return t.m_storageHandle;
		}
	};

	template<class T, typename H>
	concept HasHandle = requires(T t)
	{
		{ StorageProbe::handle(t) } -> std::same_as<H&>;
	};

	template<class T, typename Handle>
	requires(HasHandle<T, Handle>)
	class Storage
	{
	public:
		virtual ~Storage() = default;

		template<typename... Args>
		T& Add(Handle handle, Args&&... args)
		{
			TD_CORE_ASSERT(!m_indices.contains(handle));
			m_items.emplace_back(std::forward<Args>(args)...);
			auto index = m_items.size() - 1;
			m_indices.emplace(handle, index);
			return m_items[index];
		}
		T& Get(Handle handle)
		{
			TD_CORE_ASSERT(m_indices.contains(handle));
			auto it = m_indices.find(handle);
			return m_items[it->second];
		}
		std::vector<T>& GetAll()
		{
			return m_items;
		}
		T Destroy(Handle handle)
		{
			TD_CORE_ASSERT(m_indices.contains(handle));
			
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
			m_indices[StorageProbe::handle(back)] = index;
			m_items.pop_back();
			return item;
		}
		void DestroyAll()
		{
			m_items.clear();
			m_indices.clear();
		}

	protected:
		std::vector<T> m_items;
		std::unordered_map<Handle, size_t> m_indices;
	};
}