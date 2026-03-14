#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"

TD_COMPONENT(
	(TD_NAME(Button), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(int32_t, layer),
		TD_COMPONENT_FIELD(std::string, text),
		TD_COMPONENT_FIELD(float, size),
		TD_COMPONENT_FIELD(Anchor, anchor, TD_INIT(Anchor::CENTER)),
		TD_COMPONENT_FIELD(glm::fvec2, padding),
		TD_COMPONENT_FIELD(glm::u8vec4, background)
	)
)
	public:
		void StartBefore() override;
		void Update(float delta) override;

	private:
		ComponentHandle<ColorRenderer> m_colorRenderer;
		ComponentHandle<TextRenderer> m_textRenderer;
	};
}

