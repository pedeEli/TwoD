#include "tdpch.hpp"
#include "TextRenderer.hpp"

#include "TwoD/Core/App.hpp"
#include "MSDFData.hpp"

namespace TwoD
{
	void TextRenderer::StartBefore()
	{
		SetGlyphs();
	}

	void TextRenderer::Destroy()
	{
		App::Get<RenderSystem>().SetDirty<TextRenderer>();
	}

	void TextRenderer::SetText(const std::string& text)
	{
		this->text = text;
		SetGlyphs();
	}
	const std::vector<TextRenderer::Glyph> &TextRenderer::GetGlyphs() const
	{
		return m_glyphs;
	}

	void TextRenderer::SetGlyphs()
	{
		m_glyphs.clear();
		m_glyphs.reserve(text.size());
		
		auto* data = font->GetMSDFData();
		auto& metrics = data->fontGeometry.getMetrics();
		auto atlasSize = font->GetAtlasSize();
		auto width = static_cast<double>(atlasSize.x);
		auto height = static_cast<double>(atlasSize.y);

		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double x = 0;
		for (size_t i = 0; i < text.size(); i++)
		{
			auto* glyph = data->fontGeometry.getGlyph(text[i]);

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);

			m_glyphs.emplace_back<glm::fvec2, glm::fvec2, glm::fvec2, glm::fvec2>(
				{ al / width, at / height },
				{ ar / width, ab / height },
				{ x + pl * fsScale, metrics.lineHeight - pt * fsScale },
				{ x + pr * fsScale, metrics.lineHeight - pb * fsScale }
			);

			if (i + 1 < text.size())
			{
				double advance = glyph->getAdvance();
				data->fontGeometry.getAdvance(advance, text[i], text[i + 1]);
				x += advance * fsScale;
			}
		}

		App::Get<RenderSystem>().SetDirty<TextRenderer>();
	}
}