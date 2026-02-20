#pragma once
#include <typeindex>

#include "AssetDefines.hpp"
#include "AssetStorage.hpp"
#include "Asset.hpp"

#include "TwoD/Core/Storage.hpp"
#include "TwoD/SDL/Window.hpp"

namespace TwoD
{
	class AssetManager
	{
	public:
		AssetManager() = delete;
		~AssetManager() = delete;
		AssetManager(AssetManager& other) = delete;
		AssetManager(AssetManager&& other) = delete;
		AssetManager& operator=(AssetManager& other) = delete;
		AssetManager& operator=(AssetManager&& other) = delete;

		static void Init(const Window& window);
		static void Shutdown();
		
		template<typename T>
		requires(std::is_base_of_v<Asset, T>)
		static void Register(const std::string& name)
		{
			TD_CORE_ASSERT(!m_storages.contains(typeid(T)), "Cannot register assets twice!");
			static AssetStorageImpl<T> storage;
			m_storages[typeid(T)] = &storage;
			m_storagesStr[name] = &storage;
		}

		template<typename T>
		requires(std::is_base_of_v<Asset, T>)
		static T& Get(const std::string& name)
		{
			return GetStorage<T>()->Get(name);
		}

	private:
		struct Callbacks
		{
			std::function<void()> Load;
			std::function<void(const Window&)> Init;
		};

	private:
		static const Callbacks LoadFile(const std::filesystem::path& path);
		
		template<typename T>
		requires(std::is_base_of_v<Asset, T>)
		static AssetStorageImpl<T>* GetStorage()
		{
			TD_CORE_ASSERT(m_storages.contains(typeid(T)));
			return static_cast<AssetStorageImpl<T>*>(m_storages[typeid(T)]);
		}
		static AssetStorage* GetStorage(const std::string& name);

	private:
		static inline std::unordered_map<std::type_index, AssetStorage*> m_storages;
		static inline std::unordered_map<std::string, AssetStorage*> m_storagesStr;
	};
}

#include "Asset.inl"
#include "AssetStorage.inl"