#include "tdpch.hpp"
#include "Component.hpp"
#include "ECS.hpp"

namespace TwoD
{
	Entity& Component::GetEntity() const
	{
		return m_ecs->GetEntity(m_handle);
	}
}