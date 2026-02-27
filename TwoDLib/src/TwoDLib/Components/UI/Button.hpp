#pragma once
#include "TwoD.hpp"

#include "TwoDLib/Components/ColorRenderer.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"

namespace TwoD
{
	class Button : public Component
	{
		using Component::Component;
	public:
		void StartBefore() override;
		void Update(float delta) override;

	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD(int32_t, layer),
			TD_COMPONENT_FIELD(std::string, text),
			TD_COMPONENT_FIELD(float, size),
			TD_COMPONENT_FIELD(Anchor, anchor, Anchor::CENTER),
			TD_COMPONENT_FIELD(glm::fvec2, padding),
			TD_COMPONENT_FIELD(glm::u8vec4, background)
		)

	private:
		ComponentHandle<ColorRenderer> m_colorRenderer;
		ComponentHandle<TextRenderer> m_textRenderer;
	};
}

