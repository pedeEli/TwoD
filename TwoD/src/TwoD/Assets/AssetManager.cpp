#include "tdpch.hpp"
#include "AssetManager.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void AssetManager::Init(const Window& window)
	{
		auto& paths = App::GetBasePaths();

		std::vector<Callbacks> callbacks;

		for (const auto& path : paths)
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
			{
				if (entry.is_regular_file() && entry.path().extension().string() == ".yaml")
				{
					auto callback = LoadFile(entry.path());
					callbacks.push_back(callback);
				}
			}
		}

		for (auto& callback : callbacks)
		{
			callback.Load();
		}

		for (auto& callback : callbacks)
		{
			callback.Init(window);
		}
	}

	void AssetManager::Shutdown()
	{
		for (auto& storage : m_storages)
		{
			storage.second->DestroyAll();
		}
	}

	const AssetManager::Callbacks AssetManager::LoadFile(const std::filesystem::path& path)
	{
		YAML::Node node = YAML::LoadFile(path.string());

		TD_CORE_ASSERT(node["type"]);
		auto type = node["type"].as<std::string>();
		auto storage = GetStorage(type);

		TD_CORE_ASSERT(node["name"]);
		auto name = node["name"].as<std::string>();
		auto& asset = storage->Add(name);

		return Callbacks{
			.Load = [&asset, node]() { asset.Load(node); },
			.Init = [&asset, path](const Window& window) { asset.Init(path, window); }
		};
	}

	AssetStorage* AssetManager::GetStorage(const std::string& name)
	{
		TD_CORE_ASSERT(m_storagesStr.contains(name));
		return m_storagesStr[name];
	}
}