#pragma once

#include "TwoD/Core/YAML.hpp"
#include "TwoD/Core/Ref.hpp"
#include "TwoD/Core/Storage.hpp"
#include "Entity.hpp"
#include "ComponentStorage.hpp"

namespace TwoD
{
	class ECS;

	template<typename T>
	struct ComponentField
	{
		const char* name;
		T* ptr;
	};

	class Component : public HasHandle<EntityHandle>
	{
	public:
		Component(ECS* ecs, EntityHandle handle) : m_ecs(ecs), m_handle(handle) {}
		virtual ~Component() = default;

		virtual void Load(const YAML::Node& node) {}
		virtual void StartBefore() {}
		virtual void Start() {}
		virtual void Destroy() {}
		virtual void UpdateBefore(float delta) {}
		virtual void Update(float delta) {}

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		Ref<T> AddComponent() const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		Ref<T> GetComponent() const;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		void DestroyComponent() const;

		Ref<Entity> GetEntity() const;

		EntityHandle GetHandle() const override
		{
			return m_handle;
		}

	private:
		EntityHandle m_handle;
		ECS* m_ecs;
		bool m_started = false;

		template<typename T>
		requires(std::is_base_of_v<Component, T>)
		friend class ComponentStorageImpl;
	};
}