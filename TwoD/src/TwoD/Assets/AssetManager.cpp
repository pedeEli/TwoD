#include "tdpch.hpp"
#include "AssetManager.hpp"

#include "TwoD/Core/App.hpp"
#include "TwoD/Serialization/Serialization.hpp"

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
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		size_t size = file.tellg();
		file.seekg(0);
		std::vector<char> buffer(size + 1);
		file.read(buffer.data(), size);
		buffer[size] = '\0';
		Deserializer deserializer(path.string(), buffer.data());

		TD_CORE_ASSERT(deserializer["type"]);
		std::string type;
		if (!deserializer["type"].As(type))
		{
			TD_CORE_ASSERT(false, std::format("failed to load asset: {}", path.string()));
		}
		auto storage = GetStorage(type);

		std::string name;
		if (!deserializer["name"].As(name))
		{
			TD_CORE_ASSERT(false, std::format("failed to load asset: {}", path.string()));
		}
		auto& asset = storage->Add(name);

		return Callbacks{
			.Load = [&asset, deserializer]() { asset.Load(deserializer); },
			.Init = [&asset, path](const Window& window) { asset.Init(path, window); }
		};
	}

	AssetStorage* AssetManager::GetStorage(const std::string& name)
	{
		TD_CORE_ASSERT(m_storagesStr.contains(name));
		return m_storagesStr[name];
	}
}