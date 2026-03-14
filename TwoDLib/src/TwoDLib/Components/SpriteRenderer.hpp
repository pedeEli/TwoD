#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Assets/Sprite.hpp"

TD_COMPONENT(
	(TD_NAME(SpriteRenderer), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(int32_t, layer),
		TD_COMPONENT_FIELD(Sprite*, sprite),
		TD_COMPONENT_FIELD(std::optional<uint32_t>, slice, TD_INIT({}))
	)
)
	};
}