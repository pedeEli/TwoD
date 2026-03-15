#include "tdpch.hpp"
#include "Button.hpp"

namespace TwoD
{
	void Button::StartBefore()
	{
		auto& colorRenderer = AddComponent<ColorRenderer>();
		colorRenderer.layer = layer;
		colorRenderer.color = background;
		m_colorRenderer = colorRenderer;

		auto& textRenderer = AddComponent<TextRenderer>();
		textRenderer.font = &AssetManager::Get<Font>("Roboto");
		textRenderer.SetText(text);
		textRenderer.layer = layer + 1;
		textRenderer.size = size;
		m_textRenderer = textRenderer;
		auto textSize = textRenderer.GetSize();

		auto& transform = GetComponent<UITransform>();
		transform.size = textSize * size + padding * 2.0f;
		transform.anchor = anchor;
	}

	void Button::Update([[maybe_unused]] float delta)
	{
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