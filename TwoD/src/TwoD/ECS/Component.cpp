#include "tdpch.hpp"
#include "Component.hpp"
#include "ECS.hpp"

namespace TwoD
{
	Ref<Entity> Component::GetEntity() const
	{
		return m_ecs->GetEntity(m_handle);
	}
}