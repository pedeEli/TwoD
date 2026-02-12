#pragma once

namespace TwoD
{
	template<class T>
	requires(std::is_base_of_v<Asset, T>)
	T& AssetStorageImpl<T>::Add(std::string& name)
	{
		TD_CORE_ASSERT(!m_assets.contains(name), "Asset already exists!");
		return m_assets.try_emplace(name).first->second;
	}

	template<class T>
	requires(std::is_base_of_v<Asset, T>)
	T& AssetStorageImpl<T>::Get(const std::string& name)
	{
		TD_CORE_ASSERT(m_assets.contains(name));
		return m_assets.find(name)->second;
	}

	template<class T>
	requires(std::is_base_of_v<Asset, T>)
	void AssetStorageImpl<T>::Remove(const std::string& name)
	{
		auto it = m_assets.find(name);
		if (it != m_assets.end())
		{
			m_assets.erase(it);
		}
	}

	template<class T>
	requires(std::is_base_of_v<Asset, T>)
	void AssetStorageImpl<T>::RemoveAll()
	{
		m_assets.clear();
	}
}