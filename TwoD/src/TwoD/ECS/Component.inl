#pragma once

namespace TwoD
{
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	T& Component::AddComponent() const
	{
		return ECS::AddComponent<T>(m_storageHandle);
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	T& Component::GetComponent() const
	{
		return ECS::GetComponent<T>(m_storageHandle);
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	T* Component::TryGetComponent() const
	{
		return ECS::TryGetComponent<T>(m_storageHandle);
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	void Component::DestroyComponent() const
	{
		ECS::DestroyComponent<T>(m_storageHandle);
	}

	template<class T>
	requires(std::is_base_of_v<Component, T>)
	std::vector<T>& Component::GetComponents() const
	{
		return ECS::GetComponents<T>();
	}


	template<class T>
	requires(std::is_base_of_v<Component, T>)
	bool ComponentHandle<T>::operator==(const ComponentHandle<T> other) const noexcept
	{
		return other.m_handle == m_handle;
	}
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	bool ComponentHandle<T>::operator!=(const ComponentHandle<T> other) const noexcept
	{
		return other.m_handle != m_handle;
	}
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	ComponentHandle<T>::operator bool() const noexcept
	{
		return m_handle;
	}
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	T& ComponentHandle<T>::operator*() const noexcept
	{
		TD_CORE_ASSERT(m_handle);
		return ECS::GetComponent<T>(m_handle);
	}
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	T* ComponentHandle<T>::operator->() const noexcept
	{
		TD_CORE_ASSERT(m_handle);
		return &ECS::GetComponent<T>(m_handle);
	}
}