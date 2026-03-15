#include "tdpch.hpp"
#include "TextRenderer.hpp"
#include "TwoDLib/Assets/MSDFData.hpp"

namespace TwoD
{
	void TextRenderer::SetText(const std::string& newText)
	{
		text = newText;
		SetGlyphs();
	}
	const std::vector<TextRenderer::Glyph> &TextRenderer::GetGlyphs() const
	{
		return m_glyphs;
	}
	glm::fvec2 TextRenderer::GetSize() const
	{
		return m_size;
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
		double x = 0.0;
		double maxPT = 0.0;

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
				{ x + pl * fsScale, -pt * fsScale },
				{ (pr - pl) * fsScale, (pt - pb) * fsScale }
			);

			maxPT = std::max(maxPT, pt * fsScale);

			if (i + 1 < text.size())
			{
				double advance = glyph->getAdvance();
				data->fontGeometry.getAdvance(advance, text[i], text[i + 1]);
				x += advance * fsScale;
			}
			else
			{
				x += (pr - pl) * fsScale;
			}
		}

		m_size = { x, maxPT };
		for (auto& glyph : m_glyphs)
		{
			glyph.quadMin.x -= m_size.x * 0.5f;
			glyph.quadMin.y += m_size.y * 0.5f;
		}
	}
}