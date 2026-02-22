#pragma once

namespace TwoD
{
	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	T& Entity::AddComponent() const
	{
		return ECS::AddComponent<T>(m_storageHandle);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	T& Entity::GetComponent() const
	{
		return ECS::GetComponent<T>(m_storageHandle);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	T* Entity::TryGetComponent() const
	{
		return ECS::TryGetComponent<T>(m_storageHandle);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	void Entity::DestroyComponent() const
	{
		ECS::DestroyComponent<T>(m_storageHandle);
	}
}