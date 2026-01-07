#include "tdpch.hpp"
#include "SpriteAtlas.hpp"

#include <SDL3/SDL.h>
#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void SpriteAtlas::Pack()
	{
		uint32_t atlasWidth = 1024;
		uint32_t atlasHeight = 1024;
		float atlasWidthF = (float)atlasWidth;
		float atlasHeightF = (float)atlasHeight;

		uint32_t currentX = 0;
		uint32_t currentY = 0;
		uint32_t rowHeight = 0;

		std::vector<SDL::Rect> spritePositions;
		spritePositions.reserve(m_sprites.size());

		for (auto& sprite : m_sprites)
		{
			auto width = sprite.first.GetWidth();
			auto height = sprite.first.GetHeight();
			if (currentX + width > atlasWidth)
			{
				currentX = 0;
				currentY += rowHeight + gap;
				rowHeight = 0;
			}

			spritePositions.emplace_back(currentX, currentY, width, height);
			currentX += width + gap;
			rowHeight = std::max(rowHeight, height);
		}


		SDL::Surface surface(atlasWidth, atlasHeight);
		for (size_t i = 0; i < spritePositions.size(); i++)
		{
			auto& pos = spritePositions[i];
			m_sprites[i].second({ pos.x / atlasWidthF, pos.y / atlasHeightF, pos.w / atlasWidthF, pos.h / atlasHeightF });
			m_sprites[i].first.BlitTo(surface, pos);
		}
		m_sprites.clear();

		surface.SaveBMP(name + ".bmp");

		auto& window = App::Get<Window>();

		SDL::SamplerInfo samplerInfo{
			.minFilter = filter,
			.magFilter = filter,
			.mipmapMode = SDL::SamplerMipmapMode::NEAREST,
			.addressModeU = SDL::SamplerAddressMode::CLAMP_TO_EDGE,
			.addressModeV = SDL::SamplerAddressMode::CLAMP_TO_EDGE,
			.addressModeW = SDL::SamplerAddressMode::CLAMP_TO_EDGE
		};
		m_sampler = window.CreateSampler(samplerInfo);

		SDL::TextureInfo textureInfo{
			.type = SDL::TextureType::TWO_D,
			.format = textureFormat,
			.usage = SDL::TextureUsageFlags::SAMPLER,
			.width = atlasWidth,
			.height = atlasHeight,
			.layerCountOrDepth = 1,
			.numLevels = 1
		};
		m_texture = window.CreateTexture(textureInfo);
		m_texture.SetName(name);

		SDL::TransferBufferInfo transferInfo{
			.usage = SDL::TransferBufferUsage::UPLOAD,
			.size = atlasWidth * atlasHeight * 4
		};
		auto transferBuffer = window.CreateTransferBuffer(transferInfo);

		auto* transferData = transferBuffer.Map<uint8_t>(false);
		SDL_memcpy(transferData, surface.GetPixels(), transferInfo.size);

		auto commandBuffer = window.AcquireCommandBuffer();
		auto copyPass = commandBuffer.BeginCopyPass();

		SDL::TextureTransferInfo source{
			.transferBuffer = &transferBuffer,
			.offset = 0
		};
		SDL::TextureRegion destination{
			.texture = &m_texture,
			.w = atlasWidth,
			.h = atlasHeight,
			.d = 1
		};
		copyPass.UploadToTexture(source, destination, false);
	}

	void SpriteAtlas::Bind(SDL::RenderPass* renderPass)
	{
		renderPass->BindFragmentSamplers(0, { {&m_texture, &m_sampler} });
	}
}