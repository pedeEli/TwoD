#pragma once
#include "AssetDefines.hpp"
#include "TwoD/Core/YAML.hpp"

namespace TwoD
{
	class Asset
	{
	public:
		Asset() = default;
		virtual ~Asset() = default;
		Asset(const Asset& other) = delete;
		Asset(Asset&& other) = delete;
		Asset& operator=(const Asset& other) = delete;
		Asset& operator=(Asset&& other) = delete;

		virtual void Load(const YAML::Node& node) = 0;
		virtual void Init(const std::filesystem::path& path) {}
	};
}

namespace YAML
{
	template<class T>
	requires(std::is_base_of_v<TwoD::Asset, T>)
	struct convert<T*>
	{
		using pointer = T*;
		static bool decode(const Node& node, pointer& rhs);
	};
}