#pragma once
#include "tdpch.hpp"
#include "ComponentStorage.hpp"

namespace TwoD
{
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	void ComponentStorageImpl<T>::StartBefore()
	{
		if (!m_unstartedItems)
		{
			return;
		}
		for (auto& item : storage::m_items)
		{
			if (!ComponentProbe::started(item))
			{
				item.StartBefore();
			}
		}
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	void ComponentStorageImpl<T>::Start()
	{
		if (!m_unstartedItems)
		{
			return;
		}
		for (auto& item : storage::m_items)
		{
			if (!ComponentProbe::started(item))
			{
				ComponentProbe::started(item, true);
				item.Start();
			}
		}
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	Component& ComponentStorageImpl<T>::AddComponent(EntityHandle entity)
	{
		auto& ref = Add(entity);
		return static_cast<Component&>(ref);
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	Component& ComponentStorageImpl<T>::GetComponent(EntityHandle entity)
	{
		auto& ref = storage::Get(entity);
		return static_cast<Component&>(ref);
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	void ComponentStorageImpl<T>::Destroy(EntityHandle entity)
	{
		if (storage::m_indices.contains(entity))
		{
			auto component = storage::Destroy(entity);
			component.Destroy();
		}
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	void ComponentStorageImpl<T>::DestroyAll()
	{
		auto destroyed = storage::DestroyAll();
		for (auto& component : destroyed)
		{
			component.Destroy();
		}
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	void ComponentStorageImpl<T>::UpdateBefore(float delta)
	{
		for (T& item : storage::m_items)
		{
			item.UpdateBefore(delta);
		}
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	void ComponentStorageImpl<T>::Update(float delta)
	{
		for (T& item : storage::m_items)
		{
			item.Update(delta);
		}
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	T& ComponentStorageImpl<T>::Add(EntityHandle entity)
	{
		m_unstartedItems = true;
		entity->m_components.push_back(typeid(T));
		return storage::Add(entity, entity);
	}


	template<class T>
	requires(std::is_base_of_v<Component, T>)
	const void* ComponentStorageImpl<T>::CreateLoadData(const YAML::Node& node) const
	{
		if constexpr (requires(T::internal_load_data* d, const YAML::Node& n) {
			{ T::CreateLoadData(d, n) };
		})
		{
			auto* loadData = new T::internal_load_data();
			T::CreateLoadData(loadData, node);
			return loadData;
		}
		return nullptr;
	}
}