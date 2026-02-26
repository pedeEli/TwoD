#pragma once
#include <concepts>
#include "TwoD/Core/Base.hpp"

namespace TwoD
{
	class Asset;
	class AssetStorage;
	template<typename T>
	requires(std::is_base_of_v<Asset, T>)
	class AssetStorageImpl;
	class Scene;
}

#define TD_INTERNAL_ASSET_FIELD(values) TD_CHOOSE_MACRO_3(TD_INTERNAL_ASSET_FIELD_NO_DEFAULT, TD_INTERNAL_ASSET_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_ASSET_FIELD_WITH_DEFAULT(type, name, value) type name = value;
#define TD_INTERNAL_ASSET_FIELD_NO_DEFAULT(type, name) type name;

#define TD_INTERNAL_ASSET_LOAD_FIELD(values) TD_CHOOSE_MACRO_3(TD_INTERNAL_ASSET_LOAD_FIELD_NO_DEFAULT, TD_INTERNAL_ASSET_LOAD_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_ASSET_LOAD_FIELD_WITH_DEFAULT(type, name, value) if (node[#name]) { name = node[#name].as<type>(); }
#define TD_INTERNAL_ASSET_LOAD_FIELD_NO_DEFAULT(type, name) TD_CORE_ASSERT(node[#name]); \
	name = node[#name].as<type>();

#define TD_INTERNAL_ASSET_LOAD(...) void Load(const YAML::Node& node) override { \
		TD_APPLY_EACH(TD_INTERNAL_ASSET_LOAD_FIELD, __VA_ARGS__) \
	}

#define TD_ASSET(...) TD_APPLY_EACH(TD_INTERNAL_ASSET_FIELD, __VA_ARGS__) \
	TD_INTERNAL_ASSET_LOAD(__VA_ARGS__)

#define TD_ASSET_FIELD(...) (__VA_ARGS__)