#include "tdpch.hpp"
#include "TextRenderer.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void TextRenderer::StartBefore()
	{
		SetRects();
	}

	void TextRenderer::Destroy()
	{
		App::Get<RenderSystem>().SetDirty<TextRenderer>();
	}

	void TextRenderer::SetText(const std::string& text)
	{
		this->text = text;
		SetRects();
	}
	const std::vector<std::pair<SpriteRect, GlyphRect>> &TextRenderer::GetRects() const
	{
		return m_rects;
	}

	void TextRenderer::SetRects()
	{
		m_rects.clear();
		m_rects.reserve(text.size());
		int32_t advance = 0;
		for (auto ch : text)
		{
			auto& info = font->GetRect(ch);
			GlyphRect size = {
				.x = info.second.minx + advance,
				.y = info.second.miny,
				.w = info.second.maxx - info.second.minx,
				.h = info.second.maxy - info.second.miny
			};
			advance += info.second.advance;
			m_rects.push_back({ info.first, size });
		}

		App::Get<RenderSystem>().SetDirty<TextRenderer>();
	}
}