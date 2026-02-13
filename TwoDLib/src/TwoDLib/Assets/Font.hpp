#pragma once
#include "TwoD/Assets/AssetManager.hpp"
#include "SpriteAtlas.hpp"


namespace TwoD
{
	struct MSDFData;

	class Font : public Asset
	{
	public:
		struct Glyph
		{
			SpriteRect rect{};
			float width = 0.0f;;
			float height = 0.0f;
			float advance = 0.0f;
		};

	public:
		Font();
		~Font();

		void Init(const std::filesystem::path& path) override;

		const MSDFData* GetMSDFData() const
		{
			return m_data.get();
		}
		void Bind(SDL::RenderPass* renderPass);
		glm::ivec2 GetAtlasSize() const
		{
			return m_atlasSize;
		}

	public:
		TD_ASSET(
			TD_ASSET_FIELD(std::string, file)
		)

		SDL::TextureSamplerBinding binding;

	private:
		glm::ivec2 m_atlasSize;
		SDL::Texture m_texture;
		SDL::Sampler m_sampler;
		std::unique_ptr<MSDFData> m_data = nullptr;
	};
}