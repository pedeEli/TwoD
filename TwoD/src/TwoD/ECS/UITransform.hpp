#pragma once
#include "ECSDefines.hpp"
#include "Transform.hpp"
#include "TwoD/Math/Rect.hpp"

TD_ENUM(
	(TD_NAMESPACE(TwoD), TD_NAME(Anchor), TD_BASE(uint8_t)),
	(
		TD_ENUM_FIELD(CENTER, TD_INIT(0)),
		TD_ENUM_FIELD(TOP, TD_INIT(1)),
		TD_ENUM_FIELD(RIGHT, TD_INIT(2)),
		TD_ENUM_FIELD(BOTTOM, TD_INIT(4)),
		TD_ENUM_FIELD(LEFT, TD_INIT(8)),
		TD_ENUM_FIELD(TOP_RIGHT, TD_INIT(TOP | RIGHT), TD_NO_DEBUG),
		TD_ENUM_FIELD(TOP_LEFT, TD_INIT(TOP | LEFT), TD_NO_DEBUG),
		TD_ENUM_FIELD(BOTTOM_RIGHT, TD_INIT(BOTTOM | RIGHT), TD_NO_DEBUG),
		TD_ENUM_FIELD(BOTTOM_LEFT, TD_INIT(BOTTOM | LEFT), TD_NO_DEBUG)
	)
)

TD_COMPONENT(
	(TD_NAME(UITransform), TD_NAMESPACE(TwoD), TD_BASE(Transform)),
	(
		TD_COMPONENT_FIELD(glm::fvec2, size, TD_INIT({ 0.0f, 0.0f })),
		TD_COMPONENT_FIELD(glm::fvec2, offset, TD_INIT({ 0.0f, 0.0f })),
		TD_COMPONENT_FIELD(Anchor, anchor, TD_INIT(Anchor::CENTER))
	)
)
	public:
		void UpdateMatrix(const std::vector<PropagationCallback>& callbacks, glm::fmat3x3* parent = nullptr);
		Rect<float> GetRect() const;

	private:
		void UpdateMatrix(const std::vector<PropagationCallback>& callbacks, glm::fmat3x3* parent, glm::fvec2 parentSize);
	};
}

