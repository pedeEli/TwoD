#pragma once
#include "TwoD.hpp"
#include "Camera.hpp"

TD_COMPONENT(
	(TD_NAME(Hitbox), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(float, x, TD_INIT(-0.5f)),
		TD_COMPONENT_FIELD(float, y, TD_INIT(-0.5f)),
		TD_COMPONENT_FIELD(float, w, TD_INIT(1.0f)),
		TD_COMPONENT_FIELD(float, h, TD_INIT(1.0f)),
		TD_COMPONENT_FIELD(int32_t, layer, TD_INIT(0))
	)
)
	public:
		static const Hitbox* Hit(const glm::fvec2& pos);

	public:
		bool IsInside(const glm::fvec2& pos) const;
	};
}