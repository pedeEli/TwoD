#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"

TD_COMPONENT(
	(TD_NAME(Text), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(glm::fvec2, padding, TD_INIT({ 0.0f, 0.0f }))
	)
)
	public:
		void Start() override;
		void Update(float delta) override;

	private:
		ComponentHandle<TextRenderer> m_textRenderer;
		ComponentHandle<UITransform> m_transform;
	};
}