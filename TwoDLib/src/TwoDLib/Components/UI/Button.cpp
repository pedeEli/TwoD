#include "tdpch.hpp"
#include "Button.hpp"

namespace TwoD
{
	void Button::StartBefore()
	{
		auto& colorRenderer = AddComponent<ColorRenderer>();
		colorRenderer.SetLayer(layer);
		colorRenderer.r = background.r;
		colorRenderer.g = background.g;
		colorRenderer.b = background.b;
		colorRenderer.a = background.a;
		m_colorRenderer = colorRenderer;

		auto& textRenderer = AddComponent<TextRenderer>();
		textRenderer.font = &AssetManager::Get<Font>("Roboto");
		textRenderer.SetText(text);
		textRenderer.SetLayer(layer + 1);
		textRenderer.size = size;
		m_textRenderer = textRenderer;
		auto textSize = textRenderer.GetSize();

		auto& rectTransform = AddComponent<RectTransform>();
		rectTransform.size = textSize * size + padding * 2.0f;
		rectTransform.anchor = anchor;
		m_rectTransform = rectTransform;
	}
}