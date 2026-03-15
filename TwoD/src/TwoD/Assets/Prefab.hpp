#pragma once
#include "AssetDefines.hpp"
#include "AssetManager.hpp"
#include "Scene.hpp"

namespace TwoD
{
	class Prefab : public Asset
	{
	public:
		TD_ASSET(
			TD_ASSET_FIELD(EntityInfo, prefab)
		)
	};
}