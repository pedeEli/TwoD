#pragma once

#include "AssetDefines.hpp"
#include "TwoD/Core/Storage.hpp"

namespace TwoD
{
	class AssetStorage
	{
	public:
		AssetStorage() = default;
		virtual ~AssetStorage() = default;
		AssetStorage(const AssetStorage& other) = delete;
		AssetStorage(AssetStorage&& other) = delete;
		AssetStorage& operator=(const AssetStorage& other) = delete;
		AssetStorage& operator=(AssetStorage&& other) = delete;

		virtual Asset& Add(std::string& name) = 0;
		virtual Asset& Get(const std::string& name) = 0;
		virtual void Destroy(const std::string& name) = 0;
		virtual void DestroyAll() = 0;
	};

	template<typename T>
	requires(std::is_base_of_v<Asset, T>)
	class AssetStorageImpl : public AssetStorage
	{
	public:
		T& Add(std::string& name) override;
		T& Get(const std::string& name) override;
		void Destroy(const std::string& name) override;
		void DestroyAll() override;

	private:
		std::unordered_map<std::string, T> m_assets;
	};

}