#pragma once
#include <concepts>
#include <string>
#include <typeindex>
#include <ranges>

#include "ECSDefines.hpp"
#include "TwoD/Core/Storage.hpp"

namespace TwoD
{
	class EntityHandle
	{
	public:
		EntityHandle() : id(0) {}
		~EntityHandle() = default;
		EntityHandle(const EntityHandle& other) = default;
		EntityHandle(EntityHandle&& other) = default;
		EntityHandle& operator=(const EntityHandle& other) = default;
		EntityHandle& operator=(EntityHandle&& other) = default;

		bool operator==(const EntityHandle other) const noexcept;
		bool operator!=(const EntityHandle other) const noexcept;
		operator bool() const noexcept;
		Entity& operator*() const noexcept;
		Entity* operator->() const noexcept;

	public:
		static EntityHandle None;

	private:
		EntityHandle(uint32_t id) : id(id) {}

	private:
		uint32_t id = 0;

		friend class ECS;
		friend struct std::hash<EntityHandle>;
	};

	class Entity
	{
	public:
		Entity(EntityHandle handle, const std::string& name) : m_storageHandle(handle), name(name) {}
		~Entity() = default;
		Entity(const Entity&) = delete;
		Entity(Entity&&) = default;
		Entity& operator=(const Entity&) = delete;
		Entity& operator=(Entity&&) = default;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		T& AddComponent() const;
		Component& AddComponent(const std::string& name) const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		T& GetComponent() const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		T* TryGetComponent() const;

		Transform* GetTransform() const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void DestroyComponent() const;

		void Destroy() const;

		const auto GetAllComponents() const
		{
			return m_components | std::views::transform([this](std::type_index type)
				{
					return GetComponentFromTypeIndex(type);
				});
		}

		operator EntityHandle() const noexcept;

	public:
		std::string name;

	private:
		std::pair<std::type_index, Component*> GetComponentFromTypeIndex(std::type_index type) const;

	private:
		EntityHandle m_storageHandle;
		std::vector<std::type_index> m_components;

		friend class ECS;
		friend struct StorageProbe;
		template<class T>
		requires(std::is_base_of_v<Component, T>)
		friend class ComponentStorageImpl;
	};
}

template<>
struct std::hash<TwoD::EntityHandle>
{
	std::size_t operator()(const TwoD::EntityHandle& handle) const noexcept;
};