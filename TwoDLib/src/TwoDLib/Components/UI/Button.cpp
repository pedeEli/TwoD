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

		auto& transform = GetComponent<UITransform>();
		transform.SetSize(textSize * size + padding * 2.0f);
		transform.SetAnchor(anchor);
	}

	void Button::Update(float delta)
	{
		auto rect = GetComponent<UITransform>().GetRect();
		auto mouse = Inputs::GetMousePosition();
		if (rect.IsInside(mouse))
		{
			m_colorRenderer->a = 100;
		}
		else
		{
			m_colorRenderer->a = 255;
		}
	}
}