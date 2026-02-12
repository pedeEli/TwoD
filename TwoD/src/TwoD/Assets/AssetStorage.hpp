#pragma once

#include "AssetDefines.hpp"
#include "TwoD/Core/Storage.hpp"

namespace TwoD
{
	class AssetStorage
	{
	public:
		virtual Asset& Add(std::string& name) = 0;
		virtual Asset& Get(const std::string& name) = 0;
		virtual void Remove(const std::string& name) = 0;
		virtual void RemoveAll() = 0;
	};

	template<typename T>
	requires(std::is_base_of_v<Asset, T>)
	class AssetStorageImpl : public AssetStorage
	{
	public:
		T& Add(std::string& name) override;
		T& Get(const std::string& name) override;
		void Remove(const std::string& name) override;
		void RemoveAll() override;

	private:
		std::unordered_map<std::string, T> m_assets;
	};

}