#pragma once

#include "TwoD/Core/Storage.hpp"

namespace TwoD
{
	class Asset;

	class AssetStorage
	{
	public:
		virtual Asset& Add(std::string& name) = 0;
		virtual Asset& Get(const std::string& name) = 0;
		virtual void Remove(const std::string& name) = 0;
	};

	template<typename T>
	requires(std::is_base_of_v<Asset, T>)
	class AssetStorageImpl : public AssetStorage
	{
	public:
		T& Add(std::string& name) override
		{
			auto it = m_assets.find(name);
			if (it != m_assets.end())
			{
				TD_CORE_ERROR("Failed to add asset: {} with name {} already exists!", typeid(T).name(), name);
				throw;
			}
			return m_assets.try_emplace(name).first->second;
		}
		T& Get(const std::string& name) override
		{
			auto it = m_assets.find(name);
			if (it == m_assets.end())
			{
				TD_CORE_ERROR("Failed to get asset: {} with name {} does not exist!", typeid(T).name(), name);
				throw;
			}
			return it->second;
		}
		void Remove(const std::string& name) override
		{
			auto it = m_assets.find(name);
			if (it == m_assets.end())
			{
				return;
			}
			m_assets.erase(it);
		}
	private:
		std::unordered_map<std::string, T> m_assets;
	};

}