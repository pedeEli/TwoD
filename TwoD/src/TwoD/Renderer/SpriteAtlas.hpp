#pragma once
#include "TwoD/SDL/Surface.hpp"
#include "TwoD/SDL/Texture.hpp"
#include "TwoD/SDL/Sampler.hpp"
#include "TwoD/Assets/AssetManager.hpp"

namespace TwoD
{
	struct SpriteRect
	{
		float u = 0.0f;
		float v = 0.0f;
		float w = 0.0f;
		float h = 0.0f;
	};

	class SpriteAtlas : public Asset
	{
	public:
		struct SpriteInfo
		{
			SDL::Surface surface;
			SDL::Rect src;
			std::function<void(SpriteRect)> callback;
		};

	public:
		~SpriteAtlas() = default;

		void Add(SDL::Surface& surface, SDL::Rect& src, std::function<void(SpriteRect)> callback)
		{
			m_sprites.emplace_back(std::move(surface), src, callback);
		}
		void Add(SDL::Surface& surface, std::function<void(SpriteRect)> callback)
		{
			m_sprites.emplace_back(
				std::move(surface),
				SDL::Rect(0, 0, surface.GetWidth(), surface.GetHeight()),
				callback
			);
		}
		
		void Pack();
		void Bind(SDL::RenderPass* renderPass);
		const glm::fvec2 GetSize() const { return { 1024.0f, 1024.0f }; }

	public:
		TD_ASSET_FIELDS(
			TD_FIELD(std::string, name),
			TD_FIELD(SDL::Filter, filter),
			TD_FIELD(SDL::PixelFormat, pixelFormat),
			TD_FIELD(SDL::TextureFormat, textureFormat),
			TD_FIELD(uint32_t, gap, 0)
		)

	private:
		std::vector<SpriteInfo> m_sprites{};
		SDL::Texture m_texture;
		SDL::Sampler m_sampler;
	};
}

