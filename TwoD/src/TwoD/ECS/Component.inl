#pragma once

namespace TwoD
{
	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	Ref<T> Component::AddComponent() const
	{
		m_ecs->AddComponent<T>(m_handle);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	Ref<T> Component::GetComponent() const
	{
		return m_ecs->GetComponent<T>(m_handle);
	}

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	void Component::DestroyComponent() const
	{
		m_ecs->DestroyComponent<T>(m_handle);
	}
}