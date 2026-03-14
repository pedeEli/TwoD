#include "tdpch.hpp"
#include "ECS.hpp"
#include "Transform.hpp"
#include "UITransform.hpp"

namespace TwoD
{
	void ECS::Shutdown()
	{
		for (auto& storage : m_storages)
		{
			storage.second->DestroyAll();
		}
	}
	
	void ECS::Update(float delta)
	{
		while (m_unstartedComponents)
		{
			m_unstartedComponents = false;
			for (auto& storage : m_storages)
			{
				storage.second->StartBefore();
			}
			for (auto& storage : m_storages)
			{
				storage.second->Start();
			}
		}

		for (auto& storage : m_storages)
		{
			storage.second->UpdateBefore(delta);
		}
		for (auto& storage : m_storages)
		{
			storage.second->Update(delta);
		}
	}

	Entity& ECS::CreateEntity(const std::string& name, const void* data)
	{
		EntityHandle handle{ m_nextEntity++ };
		auto& entity = m_entities.Add(handle, handle, name);
		auto& transform = entity.AddComponent<Transform>();
		if (data)
		{
			auto* d = static_cast<const Transform::td_load_data*>(data);
			transform.position = d->position;
			transform.rotation = d->rotation;
			transform.scale = d->scale;
		}
		return entity;
	}

	Entity& ECS::CreateUIEntity(const std::string& name, const void* data)
	{
		EntityHandle handle{ m_nextEntity++ };
		auto& entity = m_entities.Add(handle, handle, name);
		auto& transform = entity.AddComponent<UITransform>();
		if (data)
		{
			auto* d = static_cast<const UITransform::td_load_data*>(data);
			transform.position = d->position;
			transform.rotation = d->rotation;
			transform.scale = d->scale;
			transform.size = d->size;
			transform.offset = d->offset;
			transform.anchor = d->anchor;
		}
		return entity;
	}

	Entity& ECS::CreateEntity(const std::string& name, EntityHandle parent, const void* data)
	{
		auto* transform = ECS::TryGetComponent<UITransform>(parent);
		auto& entity = transform == nullptr
			? CreateEntity(name, data)
			: CreateUIEntity(name, data);
		entity.GetTransform()->SetParent(parent);
		return entity;
	}

	Entity& ECS::CreateEntity(const std::string& name, EntityHandle parent)
	{
		return CreateEntity(name, parent, nullptr);
	}

	void ECS::DestroyEntity(EntityHandle handle)
	{
		for (auto& storage : m_storages)
		{
			storage.second->Destroy(handle);
		}
		m_entities.Destroy(handle);
	}
}