#pragma once

#include "TwoD/ECS/ECS.hpp"
#include "Font.hpp"

namespace TwoD
{
	struct GlyphRect
	{
		int32_t x, y, w, h;
	};

	class TextRenderer : public Component
	{
		using Component::Component;
	public:
		void StartBefore() override;
		void Destroy() override;

		void SetText(const std::string& text);
		const std::vector<std::pair<SpriteRect, GlyphRect>>& GetRects() const;

	public:
		TD_COMPONENT_FIELDS(
			TD_FIELD(int32_t, layer, 0),
			TD_FIELD(Font*, font),
			TD_FIELD(std::string, text)
		)

	private:
		void SetRects();

	private:
		std::vector<std::pair<SpriteRect, GlyphRect>> m_rects;
	};
}