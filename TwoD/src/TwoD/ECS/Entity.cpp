#include "tdpch.hpp"
#include "Entity.hpp"
#include "ECS.hpp"

namespace TwoD
{
	EntityHandle EntityHandle::None;

	bool EntityHandle::operator==(const EntityHandle other) const noexcept
	{
		return other.id == id;
	}
	bool EntityHandle::operator!=(const EntityHandle other) const noexcept
	{
		return other.id != id;
	}
	EntityHandle::operator bool() const noexcept
	{
		return id != 0;
	}
	Entity& EntityHandle::operator*() const noexcept
	{
		return ECS::GetEntity(*this);
	}
	Entity* EntityHandle::operator->() const noexcept
	{
		return &ECS::GetEntity(*this);
	}


	Component& Entity::AddComponent(const std::string& name) const
	{
		return ECS::AddComponent(m_storageHandle, name);
	}

	void Entity::Destroy() const
	{
		ECS::DestroyEntity(m_storageHandle);
	}

	Entity::operator EntityHandle() const noexcept
	{
		return m_storageHandle;
	}

	std::pair<std::type_index, Component*> Entity::GetComponentFromTypeIndex(std::type_index type) const
	{
		return {
			type,
			&ECS::m_storages[type]->GetComponent(m_storageHandle)
		};
	}
}

std::size_t std::hash<TwoD::EntityHandle>::operator()(const TwoD::EntityHandle& handle) const noexcept
{
	return std::hash<uint32_t>{}(handle.id);
}