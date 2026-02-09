#include "tdpch.hpp"
#include "Component.hpp"
#include "ECS.hpp"

namespace TwoD
{
	Entity& Component::GetEntity() const
	{
		return ECS::GetEntity(m_storageHandle);
	}

	Entity& Component::CreateEntity(const std::string& name) const
	{
		return ECS::CreateEntity(name);
	}
}