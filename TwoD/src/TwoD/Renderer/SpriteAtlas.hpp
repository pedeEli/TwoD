#pragma once
#include "TwoD/SDL/Surface.hpp"
#include "TwoD/SDL/Texture.hpp"
#include "TwoD/SDL/Sampler.hpp"
#include "TwoD/Assets/AssetManager.hpp"

namespace TwoD
{
	struct SpriteRect
	{
		float u, v, w, h;
	};

	class SpriteAtlas : public Asset
	{
	public:
		~SpriteAtlas() = default;

		void Add(SDL::Surface& surface, std::function<void(SpriteRect)> callback)
		{
			m_sprites.emplace_back(std::move(surface), callback);
		}
		
		void Pack();
		void Bind(SDL::RenderPass* renderPass);
		const glm::fvec2 GetSize() const { return { 1024.0f, 1024.0f }; }

	public:
		TD_ASSET_FIELDS(
			TD_FIELD(std::string, name),
			TD_FIELD(SDL::Filter, filter),
			TD_FIELD(SDL::TextureFormat, textureFormat),
			TD_FIELD(uint32_t, gap, 0)
		)

	private:
		std::vector<std::pair<SDL::Surface, std::function<void(SpriteRect)>>> m_sprites;
		SDL::Texture m_texture;
		SDL::Sampler m_sampler;

	private:
		static inline std::unordered_map<std::string, SpriteAtlas> m_atlas;
	};
}

