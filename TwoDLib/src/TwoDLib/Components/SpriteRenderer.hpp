#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Assets/Sprite.hpp"

TD_COMPONENT(
	(TD_NAME(SpriteRenderer), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(int32_t, layer, TD_INIT(0)),
		TD_COMPONENT_FIELD(Sprite*, sprite, TD_INIT(nullptr)),
		TD_COMPONENT_FIELD(glm::u8vec4, color, TD_INIT({ 255, 255, 255, 255 })),
		TD_COMPONENT_FIELD(std::optional<uint32_t>, slice, TD_INIT({}))
	)
)
	};
}