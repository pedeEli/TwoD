#pragma once
#include <concepts>
#include <string>

#include "TwoD/Core/Storage.hpp"

namespace TwoD
{
	enum class EntityHandle : uint32_t {};
	static bool operator==(const EntityHandle a, const uint32_t b)
	{
		return static_cast<uint32_t>(a) == b;
	}

	class Component;
	class ECS;

	class Entity : public HasHandle<EntityHandle>
	{
	public:
		Entity(ECS* ecs, EntityHandle handle, const std::string& name) : m_ecs(ecs), m_handle(handle), name(name) {}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		T& AddComponent();
		Component& AddComponent(const std::string& name);

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		T& GetComponent() const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void DestroyComponent();

		void Destroy();

		EntityHandle GetHandle() const override
		{
			return m_handle;
		}

	public:
		std::string name;

	private:
		EntityHandle m_handle;
		ECS* m_ecs;

		friend class ECS;
	};
}