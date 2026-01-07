#include "tdpch.hpp"
#include "Font.hpp"

#include "msdfgen.h"
#include "msdfgen-ext.h"

#include "SpriteAtlas.hpp"
#include "TwoD/Core/App.hpp"

static const std::string_view s_initialChars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

namespace TwoD
{
	void Font::Init(const std::filesystem::path& path)
	{
		auto filePath = path.parent_path() / file;

		auto* ft = msdfgen::initializeFreetype();
		auto* font = msdfgen::loadFont(ft, filePath.string().c_str());
		
		auto& atlas = App::Get<AssetManager>().Get<SpriteAtlas>("font-atlas");
		for (char ch : s_initialChars)
		{
			msdfgen::Shape shape;
			msdfgen::loadGlyph(shape, font, ch, msdfgen::FONT_SCALING_EM_NORMALIZED);
			shape.normalize();
			msdfgen::edgeColoringSimple(shape, 3.0);
			msdfgen::Bitmap<float, 3> msdf(32, 32);
			msdfgen::SDFTransformation t(msdfgen::Projection(32.0, msdfgen::Vector2(0.125, 0.125)), msdfgen::Range(0.125));
			msdfgen::generateMSDF(msdf, shape, t);
			

			auto surface = m_font.GetGlyphImage(ch, SDL::ImageType::IMAGE_ALPHA);
			m_metrics[ch] = m_font.GetGlyphMetrics(ch);
			atlas.Add(surface, [ch, this](auto rect)
				{
					m_rects[ch] = { rect, m_metrics[ch] };
				});
		}
	}

	const std::pair<SpriteRect, SDL::GlyphMetrics>& Font::GetRect(char ch) const
	{
		TD_CORE_ASSERT(m_rects.contains(ch));
		return m_rects.find(ch)->second;
	}
}