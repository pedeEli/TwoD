#pragma once
#include "AssetDefines.hpp"
#include "TwoD/Core/YAML.hpp"
#include "TwoD/SDL/Window.hpp"

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
		virtual void Init(const std::filesystem::path& path, const Window& window) {}
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