#pragma once
#include <concepts>

namespace TwoD
{
	class Asset;
	class AssetStorage;
	template<typename T>
	requires(std::is_base_of_v<Asset, T>)
	class AssetStorageImpl;
}