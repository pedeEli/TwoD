#pragma once
#include <concepts>
#include <vector>
#include <unordered_map>

#include "TwoD/Core/Storage.hpp"
#include "Entity.hpp"

namespace TwoD
{
	class Component;
	class ECS;

	class ComponentStorage
	{
	public:
		virtual Component& AddComponent(ECS* ecs, EntityHandle entity) = 0;

		virtual void StartBefore() = 0;
		virtual void Start() = 0;
		virtual void UpdateBefore(float delta) = 0;
		virtual void Update(float delta) = 0;
		virtual void Destroy(EntityHandle entity) = 0;
	};

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	class ComponentStorageImpl : public ComponentStorage, public Storage<T, EntityHandle>
	{
		using storage = Storage<T, EntityHandle>;
	public:
		void StartBefore() override
		{
			if (!m_unstartedItems)
			{
				return;
			}
			for (auto& item : storage::m_items)
			{
				if (!item.m_started)
				{
					item.StartBefore();
				}
			}
		}
		void Start() override
		{
			if (!m_unstartedItems)
			{
				return;
			}
			for (auto& item : storage::m_items)
			{
				if (!item.m_started)
				{
					item.m_started = true;
					item.Start();
				}
			}
		}
		Component& AddComponent(ECS* ecs, EntityHandle entity) override
		{
			auto& ref = Add(ecs, entity);
			return static_cast<Component&>(ref);
		}
		void Destroy(EntityHandle entity) override
		{
			if (storage::m_indices.contains(entity))
			{
				auto item = storage::Destroy(entity);
				item.Destroy();
			}
		}
		
		void UpdateBefore(float delta) override;
		void Update(float delta) override;
		
		T& Add(ECS* ecs, EntityHandle entity)
		{
			m_unstartedItems = true;
			return storage::Add(entity, ecs, entity);
		}

	private:
		bool m_unstartedItems = false;
	};
}