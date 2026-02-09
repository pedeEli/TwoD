#include "tdpch.hpp"
#include "ECS.hpp"
#include "Transform.hpp"

namespace TwoD
{
	Entity& ECS::CreateEntity(const std::string& name)
	{
		EntityHandle handle{ m_nextEntity++ };
		auto& entity = m_entities.Add(handle, handle, name);
		entity.AddComponent<Transform>();
		return entity;
	}
}