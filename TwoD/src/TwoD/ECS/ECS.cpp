#include "tdpch.hpp"
#include "ECS.hpp"
#include "Transform.hpp"

namespace TwoD
{
	void ECS::Destroy()
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

	Entity& ECS::CreateEntity(const std::string& name)
	{
		EntityHandle handle{ m_nextEntity++ };
		auto& entity = m_entities.Add(handle, handle, name);
		entity.AddComponent<Transform>();
		return entity;
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