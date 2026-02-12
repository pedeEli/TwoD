#pragma once
#include <vector>
#include <concepts>
#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>

#include "ECSDefines.hpp"
#include "TwoD/Core/Log.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "ComponentStorage.hpp"

namespace TwoD
{
	class ECS
	{
	public:
		ECS() = delete;
		~ECS() = delete;
		ECS(const ECS&) = delete;
		ECS(ECS&&) = delete;
		ECS& operator=(const ECS&) = delete;
		ECS& operator=(ECS&&) = delete;

		static void Destroy();
		static void Update(float delta);

		static Entity& CreateEntity(const std::string& name);
		static void DestroyEntity(EntityHandle handle);

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		static void Register(const std::string& name)
		{
			TD_CORE_ASSERT(!m_storages.contains(typeid(T)));
			static ComponentStorageImpl<T> storage;
			m_storages[typeid(T)] = &storage;
			m_storagesStr[name] = &storage;
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		static std::vector<T>& GetComponents()
		{
			return GetStorage<T>()->GetAll();
		}

		static Entity& GetEntity(EntityHandle handle)
		{
			return m_entities.Get(handle);
		}

	private:
		template<class T>
		requires(std::is_base_of_v<Component, T>)
		static ComponentStorageImpl<T>* GetStorage()
		{
			TD_CORE_ASSERT(m_storages.contains(typeid(T)));
			return static_cast<ComponentStorageImpl<T>*>(m_storages[typeid(T)]);
		}

		static ComponentStorage* GetStorage(const std::string& name)
		{
			TD_CORE_ASSERT(m_storagesStr.contains(name));
			return m_storagesStr[name];
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		static T& AddComponent(EntityHandle entity)
		{
			m_unstartedComponents = true;
			return GetStorage<T>()->Add(entity);
		}
		static Component& AddComponent(EntityHandle entity, const std::string& name)
		{
			m_unstartedComponents = true;
			return GetStorage(name)->AddComponent(entity);
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		static T& GetComponent(EntityHandle entity)
		{
			return GetStorage<T>()->Get(entity);
		}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		static void DestroyComponent(EntityHandle entity)
		{
			GetStorage<T>()->Destroy(entity);
		}

	private:
		static inline Storage<Entity, EntityHandle> m_entities;
		static inline std::unordered_map<std::type_index, ComponentStorage*> m_storages;
		static inline std::unordered_map<std::string, ComponentStorage*> m_storagesStr;
		static inline uint32_t m_nextEntity = 1;
		static inline bool m_unstartedComponents = false;

		friend class Component;
		friend class Entity;
		template<class T>
		requires(std::is_base_of_v<Component, T>)
		friend class ComponentHandle;
	};
}


#include "Entity.inl"
#include "ComponentStorage.inl"
#include "Component.inl"