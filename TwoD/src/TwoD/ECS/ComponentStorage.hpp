#pragma once
#include <concepts>
#include <vector>
#include <unordered_map>

#include "ECSDefines.hpp"
#include "TwoD/Core/Storage.hpp"
#include "Entity.hpp"
#include "Component.hpp"

namespace TwoD
{
	class ComponentStorage
	{
	public:
		virtual ~ComponentStorage() = default;
		
		virtual Component& AddComponent(EntityHandle entity) = 0;

		virtual void StartBefore() = 0;
		virtual void Start() = 0;
		virtual void UpdateBefore(float delta) = 0;
		virtual void Update(float delta) = 0;
		virtual void Destroy(EntityHandle entity) = 0;
		virtual void DestroyAll() = 0;
	};

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	class ComponentStorageImpl : public ComponentStorage, public Storage<T, EntityHandle>
	{
		using storage = Storage<T, EntityHandle>;
	public:
		ComponentStorageImpl() = default;
		~ComponentStorageImpl() = default;
		ComponentStorageImpl(const ComponentStorageImpl<T>&) = delete;
		ComponentStorageImpl(ComponentStorageImpl<T>&&) = delete;
		ComponentStorageImpl& operator=(const ComponentStorageImpl<T>&) = delete;
		ComponentStorageImpl& operator=(ComponentStorageImpl<T>&&) = delete;

		Component& AddComponent(EntityHandle entity) override;
		
		void StartBefore() override;
		void Start() override;
		void UpdateBefore(float delta) override;
		void Update(float delta) override;
		void Destroy(EntityHandle entity) override;
		void DestroyAll() override;
		
		T& Add(EntityHandle entity);

	private:
		bool m_unstartedItems = false;
	};
}