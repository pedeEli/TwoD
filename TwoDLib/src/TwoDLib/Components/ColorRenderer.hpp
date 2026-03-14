#pragma once
#include "TwoD.hpp"

TD_COMPONENT(
	(TD_NAME(ColorRenderer), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(int32_t, layer),
		TD_COMPONENT_FIELD(glm::u8vec4, color)
	)
)
	};
}