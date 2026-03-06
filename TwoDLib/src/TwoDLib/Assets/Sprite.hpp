#pragma once
#include "TwoD/Assets/AssetManager.hpp"
#include "TwoD/SDL/Surface.hpp"
#include "SpriteAtlas.hpp"

TD_STRUCT(
	(TD_NAMESPACE(TwoD), TD_NAME(SpriteSlices)),
	(
		TD_STRUCT_FIELD(uint32_t, width),
		TD_STRUCT_FIELD(uint32_t, height),
		TD_STRUCT_FIELD(uint32_t, count)
	),
)

namespace TwoD
{
	class Sprite : public Asset
	{
	public:
		~Sprite() = default;

		void Init(const std::filesystem::path& path, const Window& window) override;
		
		const SpriteRect& GetRect() const
		{
			return m_rect;
		}
		const SpriteRect& GetRect(uint32_t i) const
		{
			return m_rects[i];
		}

	public:
		TD_ASSET(
			TD_ASSET_FIELD(std::string, file),
			TD_ASSET_FIELD(std::string, name),
			TD_ASSET_FIELD(std::optional<SpriteSlices>, slices, {})
		)

	private:
		void Apply(uint32_t width, uint32_t height, SpriteRect rect, float halfPixelW, float halfPixelH);

	private:
		SpriteRect m_rect = { 0, 0, 0, 0 };
		std::vector<SpriteRect> m_rects;
	};
}