#include "tdpch.hpp"
#include "Component.hpp"
#include "ECS.hpp"
#include "UITransform.hpp"
#include "Transform.hpp"

namespace TwoD
{
	Entity& Component::GetEntity() const
	{
		return ECS::GetEntity(m_storageHandle);
	}
	Entity* Component::GetEntity(const std::string& name) const
	{
		return ECS::GetEntity(name);
	}

	Entity& Component::CreateEntity(const std::string& name) const
	{
		return ECS::CreateEntity(name, m_storageHandle);
	}

	Transform* Component::GetTransform() const
	{
		auto* transform = ECS::TryGetComponent<UITransform>(m_storageHandle);
		if (transform)
		{
			return transform;
		}
		return &ECS::GetComponent<Transform>(m_storageHandle);
	}
}