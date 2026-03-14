#pragma once
#include "TwoD.hpp"

namespace TwoD
{
	class Mesh : public Asset
	{
	public:
		TD_ASSET(
			TD_ASSET_FIELD(std::vector<Vertex>, vertices),
			TD_ASSET_FIELD(std::vector<uint32_t>, indices)
		)
	};
}
