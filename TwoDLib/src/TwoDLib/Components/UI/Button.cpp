#include "tdpch.hpp"
#include "Button.hpp"

namespace TwoD
{
	void Button::Start()
	{
		auto& colorRenderer = GetComponent<ColorRenderer>();
		colorRenderer.layer = layer;
		colorRenderer.color = background;
		m_colorRenderer = colorRenderer;

		auto& textRenderer = GetComponent<TextRenderer>();
		textRenderer.layer = layer + 1;
		m_textRenderer = textRenderer;

		m_transform = GetComponent<UITransform>();

		m_mouseClickHandle = EventHandler::Add<MouseDownEvent>([this]([[maybe_unused]] auto& event)
			{
				if (hover)
				{
					mouseClick.Emit();
				}
				return false;
			});
	}

	void Button::Destroy()
	{
		EventHandler::Remove(m_mouseClickHandle);
	}

	void Button::Update([[maybe_unused]] float delta)
	{
		auto textSize = m_textRenderer->GetSize();
		m_transform->size = textSize + padding * 2.0f;

		auto rect = GetComponent<UITransform>().GetRect();
		auto mouse = Inputs::GetMousePosition();
		hover = rect.IsInside(mouse);
	}
}