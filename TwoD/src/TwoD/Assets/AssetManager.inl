#pragma once

namespace YAML
{
	template<class T>
	requires(std::is_base_of_v<TwoD::Asset, T>)
	Node convert<T*>::encode(const pointer& rhs)
	{
		TD_CORE_ASSERT(false, "Cannot convert asset to yaml node!")
	}
	
	template<class T>
	requires(std::is_base_of_v<TwoD::Asset, T>)
	bool convert<T*>::decode(const Node& node, pointer& rhs)
	{
		auto& assetManager = TwoD::App::Get<TwoD::AssetManager>();
		rhs = &assetManager.Get<T>(node.as<std::string>());
		return true;
	}
}