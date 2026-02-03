#include "tdpch.hpp"
#include "Sprite.hpp"

#include "TwoD/Core/App.hpp"
#include "TwoDLib/RenderLayers/SpriteRenderLayer.hpp"
#include "SpriteAtlas.hpp"

namespace TwoD
{
	void Sprite::Init(const std::filesystem::path& path)
	{
		auto filePath = path.parent_path() / file;
		auto surface = SDL::Surface::LoadFile(filePath);

		auto width = surface.GetWidth();
		auto height = surface.GetHeight();

		auto& atlas = App::Get<AssetManager>().Get<SpriteAtlas>("sprite-atlas");
		atlas.Add(surface, [this, width, height](SpriteRect rect, float halfPixelW, float halfPixelH) {
			Apply(width, height, rect, halfPixelW, halfPixelH);
		});
	}

	void Sprite::Apply(uint32_t width, uint32_t height, SpriteRect rect, float halfPixelW, float halfPixelH)
	{
		m_rect.u = rect.u + halfPixelW;
		m_rect.v = rect.v + halfPixelH;
		m_rect.w = rect.w - 2 * halfPixelW;
		m_rect.h = rect.h - 2 * halfPixelH;
		uint32_t currentX = 0;
		uint32_t currentY = 0;

		if (!slices)
		{
			return;
		}

		m_rects.reserve(slices->count);
		for (size_t i = 0; i < slices->count; i++)
		{
			float u = (float)currentX / (float)width * rect.w + rect.u;
			float v = (float)currentY / (float)height * rect.h + rect.v;
			float w = (float)slices->width / (float)width * rect.w;
			float h = (float)slices->height / (float)height * rect.h;
			m_rects.emplace_back(u + halfPixelW, v + halfPixelH, w - 2 * halfPixelW, h - 2 * halfPixelH);
			
			currentX += slices->width;
			if (currentX + slices->width >= width)
			{
				currentX = 0;
				currentY += slices->height;
			}
		}
	}
}