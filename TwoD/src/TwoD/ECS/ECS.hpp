#pragma once
#include <vector>
#include <concepts>
#include <unordered_map>
#include <string>
#include <memory>

#include "TwoD/Core/Log.hpp"
#include "TwoD/Core/Ref.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "ComponentStorage.hpp"

namespace TwoD
{
	class ECS
	{
	public:
		ECS() = default;
		~ECS() = default;
		ECS(ECS& other) = delete;
		ECS(ECS&& other) = delete;
		ECS& operator=(ECS& other) = delete;
		ECS& operator=(ECS&& other) = delete;

		void Update(float delta)
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

		Ref<Entity> CreateEntity(const std::string& name);
		void DestroyEntity(EntityHandle handle)
		{
			for (auto& storage : m_storages)
			{
				storage.second->Destroy(handle);
			}
			m_entities.Destroy(handle);
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void Register()
		{
			Register<T>(typeid(T).name());
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void Register(const std::string& name)
		{
			TD_CORE_ASSERT(!m_storages.contains(name))
			m_storages[name] = std::make_unique<ComponentStorageImpl<T>>();
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		std::vector<T>& GetComponents()
		{
			return GetStorage<T>()->GetAll();
		}

	private:
		Ref<Entity> GetEntity(EntityHandle handle)
		{
			return m_entities.Get(handle);
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		ComponentStorageImpl<T>* GetStorage()
		{
			return static_cast<ComponentStorageImpl<T>*>(GetStorage(typeid(T).name()));
		}
		ComponentStorage* GetStorage(const std::string& name)
		{
			TD_CORE_ASSERT(m_storages.contains(name))
			auto storage = m_storages.find(name);
			return storage->second.get();
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		Ref<T> AddComponent(EntityHandle entity)
		{
			m_unstartedComponents = true;
			return GetStorage<T>()->Add(this, entity);
		}
		Component& AddComponent(EntityHandle entity, const std::string& name)
		{
			m_unstartedComponents = true;
			return GetStorage(name)->AddComponent(this, entity);
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		Ref<T> GetComponent(EntityHandle entity)
		{
			return GetStorage<T>()->Get(entity);
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void DestroyComponent(EntityHandle entity)
		{
			GetStorage<T>()->Destory(entity);
		}

	private:
		Storage<Entity, EntityHandle> m_entities;
		//std::unordered_map<EntityHandle, std::unique_ptr<Entity>> m_entities;
		std::unordered_map<std::string, std::unique_ptr<ComponentStorage>> m_storages;
		uint32_t m_nextEntity = 1;
		bool m_unstartedComponents = false;

		friend class Component;
		friend class Entity;
	};
}


#include "Entity.inl"
#include "ComponentStorage.inl"
#include "Component.inl"