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
		ComponentStorage() = default;
		virtual ~ComponentStorage() = default;
		ComponentStorage(const ComponentStorage& other) = delete;
		ComponentStorage(ComponentStorage&& other) = delete;
		ComponentStorage& operator=(const ComponentStorage& other) = delete;
		ComponentStorage& operator=(ComponentStorage&& other) = delete;

		virtual Component& AddComponent(EntityHandle entity, const void* data) = 0;
		virtual Component& GetComponent(EntityHandle entity) = 0;

		virtual void StartBefore() = 0;
		virtual void Start() = 0;
		virtual void UpdateBefore(float delta) = 0;
		virtual void Update(float delta) = 0;
		virtual void Destroy(EntityHandle entity) = 0;
		virtual void DestroyAll() = 0;

		virtual bool CreateLoadData(const Deserializer& deserializer, void*& value) const = 0;
		virtual bool ModifyLoadData(const Deserializer& deserializer, void*& value) const = 0;
		virtual void* CopyLoadData(const void* value) const = 0;
	};

	template<typename T>
	requires(std::is_base_of_v<Component, T>)
	class ComponentStorageImpl : public ComponentStorage, public Storage<T, EntityHandle>
	{
		using storage = Storage<T, EntityHandle>;
	public:
		ComponentStorageImpl() = default;
		~ComponentStorageImpl() = default;
		ComponentStorageImpl(const ComponentStorageImpl<T>& other) = delete;
		ComponentStorageImpl(ComponentStorageImpl<T>&& other) = delete;
		ComponentStorageImpl& operator=(const ComponentStorageImpl<T>& other) = delete;
		ComponentStorageImpl& operator=(ComponentStorageImpl<T>&& other) = delete;

		Component& AddComponent(EntityHandle entity, const void* data) override;
		Component& GetComponent(EntityHandle entity) override;
		
		void StartBefore() override;
		void Start() override;
		void UpdateBefore(float delta) override;
		void Update(float delta) override;
		void Destroy(EntityHandle entity) override;
		void DestroyAll() override;

		bool CreateLoadData(const Deserializer& deserializer, void*& value) const override;
		bool ModifyLoadData(const Deserializer& deserializer, void*& value) const override;
		void* CopyLoadData(const void* value) const override;

		
		T& Add(EntityHandle entity, const T::td_load_data* data = nullptr);

	private:
		bool m_unstartedItems = false;
	};
}