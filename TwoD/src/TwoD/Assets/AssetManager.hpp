#pragma once
#include "AssetStorage.hpp"
#include "TwoD/Core/YAML.hpp"
#include "TwoD/Core/Storage.hpp"

namespace TwoD
{
	class Asset
	{
	public:
		virtual ~Asset() = default;

		virtual void Load(const YAML::Node& node) = 0;
		virtual void Init(const std::filesystem::path& path) {}
	};

	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;
		AssetManager(AssetManager& other) = delete;
		AssetManager(AssetManager&& other) = delete;
		AssetManager& operator=(AssetManager& other) = delete;
		AssetManager& operator=(AssetManager&& other) = delete;

		void Load();
		
		template<typename T>
		requires(std::is_base_of_v<Asset, T>)
		void Register()
		{
			Register<T>(typeid(T).name());
		}
		template<typename T>
		requires(std::is_base_of_v<Asset, T>)
		void Register(const std::string& name)
		{
			m_storages.emplace(name, std::make_unique<AssetStorageImpl<T>>());
		}

		template<typename T>
		requires(std::is_base_of_v<Asset, T>)
		T& Get(const std::string& name)
		{
			return GetStorage<T>()->Get(name);
		}

	private:
		struct Callbacks
		{
			std::function<void()> Load;
			std::function<void()> Init;
		};

	private:
		const Callbacks LoadFile(const std::filesystem::path& path);
		
		template<typename T>
		requires(std::is_base_of_v<Asset, T>)
		AssetStorageImpl<T>* GetStorage() const
		{
			return static_cast<AssetStorageImpl<T>*>(GetStorage(typeid(T).name()));
		}
		AssetStorage* GetStorage(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::unique_ptr<AssetStorage>> m_storages;
	};
}

namespace YAML
{
	template<class T>
	requires(std::is_base_of_v<TwoD::Asset, T>)
	struct convert<T*>
	{
		using pointer = T*;
		static Node encode(const pointer& rhs);
		static bool decode(const Node& node, pointer& rhs);
	};
}