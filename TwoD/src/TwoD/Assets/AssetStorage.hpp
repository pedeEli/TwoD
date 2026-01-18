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
			TD_CORE_ASSERT(!m_assets.contains(name), "Asset already exists!");
			return m_assets.try_emplace(name).first->second;
		}
		T& Get(const std::string& name) override
		{
			TD_CORE_ASSERT(m_assets.contains(name));
			return m_assets.find(name)->second;
		}
		void Remove(const std::string& name) override
		{
			auto it = m_assets.find(name);
			if (it != m_assets.end())
			{
				m_assets.erase(it);
			}
		}

	private:
		std::unordered_map<std::string, T> m_assets;
	};

}