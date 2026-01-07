#include "tdpch.hpp"
#include "ECS.hpp"
#include "Transform.hpp"

namespace TwoD
{
	Ref<Entity> ECS::CreateEntity(const std::string& name)
	{
		auto handle = static_cast<EntityHandle>(m_nextEntity++);
		auto entity = m_entities.Add(handle, this, handle, name);
		entity->AddComponent<Transform>();
		return entity;
	}
}