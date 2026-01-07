#pragma once
#include "TwoD/Assets/AssetManager.hpp"
#include "SpriteAtlas.hpp"


namespace TwoD
{
	class Font : public Asset
	{
	public:
		struct GlyphMetrics
		{
			int minx, maxx, miny, maxy, advance;
		};

	public:
		Font() = default;
		~Font() = default;

		void Init(const std::filesystem::path& path) override;

		const std::pair<SpriteRect, GlyphMetrics>& GetRect(char ch) const;

	public:
		TD_ASSET_FIELDS(
			TD_FIELD(std::string, file)
		)

	private:
		std::unordered_map<char, GlyphMetrics> m_metrics;
		std::unordered_map<char, std::pair<SpriteRect, GlyphMetrics>> m_rects;
	};
}