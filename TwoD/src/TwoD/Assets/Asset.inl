#pragma once

namespace TwoD
{
	template<class T>
	requires(std::is_base_of_v<Asset, T>)
	bool Deserializable<T*>::Deserialize(const Deserializer& deserializer, T*& value)
	{
		std::string name;
		if (!deserializer.As<std::string>(name))
		{
			return false;
		}
		value = &TwoD::AssetManager::Get<T>(name);
		return true;
	}
}