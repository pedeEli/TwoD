#pragma once
#include "tdpch.hpp"
#include "ComponentStorage.hpp"

namespace TwoD
{
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
}