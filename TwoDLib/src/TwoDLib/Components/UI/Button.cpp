#include "tdpch.hpp"
#include "Button.hpp"

namespace TwoD
{
	void Button::StartBefore()
	{
		auto& colorRenderer = GetComponent<ColorRenderer>();
		colorRenderer.layer = layer;
		colorRenderer.color = background;
		m_colorRenderer = colorRenderer;

		auto& textRenderer = GetComponent<TextRenderer>();
		textRenderer.layer = layer + 1;
		m_textRenderer = textRenderer;

		m_transform = GetComponent<UITransform>();
	}

	void Button::Update([[maybe_unused]] float delta)
	{
		auto textSize = m_textRenderer->GetSize();
		m_transform->size = textSize + padding * 2.0f;

		auto rect = GetComponent<UITransform>().GetRect();
		auto mouse = Inputs::GetMousePosition();
		if (rect.IsInside(mouse))
		{
			m_colorRenderer->color.a = 100;
		}
		else
		{
			m_colorRenderer->color.a = 255;
		}
	}
}