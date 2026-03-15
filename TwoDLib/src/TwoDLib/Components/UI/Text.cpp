#include "tdpch.hpp"
#include "Text.hpp"

namespace TwoD
{
	void Text::Start()
	{
		m_textRenderer = GetComponent<TextRenderer>();
		m_transform = GetComponent<UITransform>();
	}

	void Text::Update([[maybe_unused]] float delta)
	{
		auto textSize = m_textRenderer->GetSize();
		m_transform->size = textSize + padding * 2.0f;
	}
}