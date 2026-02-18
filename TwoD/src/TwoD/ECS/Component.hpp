#pragma once
#include "ECSDefines.hpp"

#include "TwoD/Core/YAML.hpp"
#include "TwoD/Core/Storage.hpp"
#include "Entity.hpp"

namespace TwoD
{
	struct ComponentProbe
	{
		template<class T>
		requires(std::is_base_of_v<Component, T>)
		static bool started(T& t)
		{
			return t.m_started;
		}
		template<class T>
		requires(std::is_base_of_v<Component, T>)
		static void started(T& t, bool value)
		{
			t.m_started = value;
		}
	};

	class Component
	{
	public:
		Component(EntityHandle handle) : m_storageHandle(handle) {}
		virtual ~Component() = default;
		Component(const Component&) = delete;
		Component(Component&&) = default;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = default;

		virtual void Load(const void* data) {}
		virtual void StartBefore() {}
		virtual void Start() {}
		virtual void Destroy() {}
		virtual void UpdateBefore(float delta) {}
		virtual void Update(float delta) {}

#ifdef TD_IMGUI
		virtual void Debug() {};
#endif

		template<class T>
		requires(std::is_base_of_v<Component, T>)
		T& AddComponent() const;

		template<class T>
		requires(std::is_base_of_v<Component, T>)
		T& GetComponent() const;

		template<class T>
		requires(std::is_base_of_v<Component, T>)
		void DestroyComponent() const;

		template<class T>
		requires(std::is_base_of_v<Component, T>)
		std::vector<T>& GetComponents() const;

		Entity& GetEntity() const;
		Entity& CreateEntity(const std::string& name) const;

	private:
		EntityHandle m_storageHandle;
		bool m_started = false;

		friend struct ComponentProbe;
		friend struct StorageProbe;
		template<class T>
		requires(std::is_base_of_v<Component, T>)
		friend class ComponentHandle;
	};

	/**
	 * Everytime you use a ComponentHandle to access a Component it has to perform a lookup.
	 * If you use a ComponentHandle in a function that is called everyframe and you access
	 * the Component multiple times in that function, consider getting a reference of the
	 * Component using the * operator and perform the actions on the reference. That way
	 * there only is ever one lookup per frame instead of many.
	 */
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	class ComponentHandle
	{
	public:
		ComponentHandle(const T& t) : ComponentHandle(t.m_storageHandle) {}
		~ComponentHandle() = default;
		ComponentHandle(const ComponentHandle&) = default;
		ComponentHandle(ComponentHandle&&) = default;
		ComponentHandle& operator=(const ComponentHandle&) = default;
		ComponentHandle& operator=(ComponentHandle&&) = default;

		bool operator==(const ComponentHandle other) const noexcept;
		bool operator!=(const ComponentHandle other) const noexcept;
		operator bool() const noexcept;
		T& operator*() const noexcept;
		T* operator->() const noexcept;

	private:
		ComponentHandle(EntityHandle handle) : m_handle(handle) {}

	private:
		EntityHandle m_handle;

		friend class Component;
		friend class Entity;
	};
}