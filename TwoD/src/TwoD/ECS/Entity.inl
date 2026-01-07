#pragma once

namespace TwoD
{
	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	Ref<T> Entity::AddComponent()
	{
		return m_ecs->AddComponent<T>(m_handle);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	Ref<T> Entity::GetComponent() const
	{
		return m_ecs->GetComponent<T>(m_handle);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	void Entity::DestroyComponent()
	{
		m_ecs->DestroyComponent<T>(m_handle);
	}
}