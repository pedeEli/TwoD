#pragma once

namespace YAML
{
	template<class T>
	requires(std::is_base_of_v<TwoD::Asset, T>)
	bool convert<T*>::decode(const Node& node, pointer& rhs)
	{
		rhs = &TwoD::AssetManager::Get<T>(node.as<std::string>());
		return true;
	}
}