#pragma once
#include "TwoD.hpp"

TD_ENUM(
	(TD_NAME(FlexDirection), TD_NAMESPACE(TwoD)),
	(
		TD_ENUM_FIELD(ROW),
		TD_ENUM_FIELD(COLUMN)
	)
)

TD_ENUM(
	(TD_NAME(JustifyContent), TD_NAMESPACE(TwoD)),
	(
		TD_ENUM_FIELD(START),
		TD_ENUM_FIELD(END),
		TD_ENUM_FIELD(CENTER),
		TD_ENUM_FIELD(SPACE_BETWEEN),
		TD_ENUM_FIELD(SPACE_AROUND),
		TD_ENUM_FIELD(SPACE_EVENLY)
	)
)

TD_ENUM(
	(TD_NAME(AlignItems), TD_NAMESPACE(TwoD)),
	(
		TD_ENUM_FIELD(START),
		TD_ENUM_FIELD(END),
		TD_ENUM_FIELD(CENTER),
		TD_ENUM_FIELD(STRETCH)
	)
)

TD_COMPONENT(
	(TD_NAME(FlexLayout), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(FlexDirection, direction, TD_INIT(FlexDirection::ROW)),
		TD_COMPONENT_FIELD(JustifyContent, justify, TD_INIT(JustifyContent::START)),
		TD_COMPONENT_FIELD(AlignItems, align, TD_INIT(AlignItems::START)),
		TD_COMPONENT_FIELD(float, gap, TD_INIT(0.0f)),
		TD_COMPONENT_FIELD(glm::fvec2, padding, TD_INIT({ 0.0f, 0.0f }))
	)
)
	public:
		static void ApplyFlexLayout(Transform& transform);
	
	public:
		void Apply(UITransform& parent) const;
	};
}