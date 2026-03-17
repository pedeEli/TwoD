#pragma once
#include "TwoD.hpp"

TD_ENUM(
	(TD_NAME(SizingMode), TD_NAMESPACE(TwoD)),
	(
		TD_ENUM_FIELD(FIT_CONTENT),
		TD_ENUM_FIELD(FIXED),
		TD_ENUM_FIELD(PERCENTAGE)
	)
)

TD_COMPONENT(
	(TD_NAME(LayoutSize), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(glm::fvec2, minSize, TD_INIT({ 0.0f, 0.0f })),
		TD_COMPONENT_FIELD(glm::fvec2, maxSize, TD_INIT({ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() })),
		TD_COMPONENT_FIELD(glm::fvec2, size, TD_INIT({ 100.0f, 100.0f })),
		TD_COMPONENT_FIELD(SizingMode, widthMode, TD_INIT(SizingMode::FIT_CONTENT)),
		TD_COMPONENT_FIELD(SizingMode, heightMode, TD_INIT(SizingMode::FIT_CONTENT))
	)
)
	public:
		void Start() override;
		void Update(float delta) override;
	
	private:
		ComponentHandle<UITransform> m_transform;
	};
}