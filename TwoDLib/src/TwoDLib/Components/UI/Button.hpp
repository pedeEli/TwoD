#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"
#include "TwoDLib/Event.hpp"

TD_COMPONENT(
	(TD_NAME(Button), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(int32_t, layer, TD_INIT(0)),
		TD_COMPONENT_FIELD(glm::fvec2, padding, TD_INIT({ 0.0f, 0.0f })),
		TD_COMPONENT_FIELD(glm::u8vec4, background, TD_INIT({ 255, 255, 255, 255 }))
	)
)
	public:
		void StartBefore() override;
		void Destroy() override;
		void Update(float delta) override;

	public:
		Event<glm::fvec2> mouseClick;
		bool hover = false;

	private:
		ComponentHandle<ColorRenderer> m_colorRenderer;
		ComponentHandle<TextRenderer> m_textRenderer;
		ComponentHandle<UITransform> m_transform;

		EventHandler::Handle m_mouseClickHandle;
	};
}

