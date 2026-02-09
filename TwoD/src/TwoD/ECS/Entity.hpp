#pragma once
#include <concepts>
#include <string>

#include "ECSDefines.hpp"
#include "TwoD/Core/Storage.hpp"

namespace TwoD
{
	class EntityHandle
	{
	public:
		EntityHandle() : id(0) {}
		~EntityHandle() = default;
		EntityHandle(const EntityHandle&) = default;
		EntityHandle(EntityHandle&&) = default;
		EntityHandle& operator=(const EntityHandle&) = default;
		EntityHandle& operator=(EntityHandle&&) = default;

		bool operator==(const EntityHandle other) const noexcept;
		bool operator!=(const EntityHandle other) const noexcept;
		operator bool() const noexcept;
		Entity& operator*() const noexcept;
		Entity* operator->() const noexcept;

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
		void DestroyComponent() const;

		void Destroy() const;

		operator EntityHandle() const noexcept;
		
		template<class T>
		requires(std::is_base_of_v<Component, T>)
		operator ComponentHandle<T>() const;

	public:
		std::string name;

	private:
		EntityHandle m_storageHandle;

		friend class ECS;
		friend struct StorageProbe;
	};
}

template<>
struct std::hash<TwoD::EntityHandle>
{
	std::size_t operator()(const TwoD::EntityHandle& handle) const noexcept;
};