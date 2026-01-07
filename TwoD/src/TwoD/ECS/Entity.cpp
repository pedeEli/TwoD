#include "tdpch.hpp"
#include "Entity.hpp"
#include "ECS.hpp"

namespace TwoD
{
	Component& Entity::AddComponent(const std::string& name)
	{
		return m_ecs->AddComponent(m_handle, name);
	}

	void Entity::Destroy()
	{
		m_ecs->DestroyEntity(m_handle);
	}
}