#pragma once
#include "TwoD/Assets/AssetManager.hpp"
#include "TwoD/SDL/Surface.hpp"
#include "SpriteAtlas.hpp"

namespace TwoD
{
	class Sprite : public Asset
	{
	public:
		struct Slices
		{
			uint32_t width;
			uint32_t height;
			uint32_t count;
		};

	public:
		~Sprite() = default;

		void Init(const std::filesystem::path& path) override;
		
		const SpriteRect& GetRect() const
		{
			return m_rect;
		}
		const SpriteRect& GetRect(uint32_t i) const
		{
			return m_rects[i];
		}

	public:
		TD_ASSET_FIELDS(
			TD_FIELD(std::string, file),
			TD_FIELD(std::string, name),
			TD_FIELD(std::optional<Slices>, slices)
		)

	private:
		void Apply(uint32_t width, uint32_t height, SpriteRect rect, float halfPixelW, float halfPixelH);

	private:
		SpriteRect m_rect = { 0, 0, 0, 0 };
		std::vector<SpriteRect> m_rects;
	};
}

namespace YAML
{
	template<>
	struct convert<TwoD::Sprite::Slices>
	{
		static Node encode(const TwoD::Sprite::Slices& rhs)
		{
			Node node;
			node["width"] = rhs.width;
			node["height"] = rhs.height;
			node["count"] = rhs.count;
		}
		static bool decode(const Node& node, TwoD::Sprite::Slices& rhs)
		{
			auto width = node["width"];
			auto height = node["height"];
			auto count = node["count"];
			TD_CORE_ASSERT(width);
			TD_CORE_ASSERT(height);
			TD_CORE_ASSERT(count);
			rhs.width = width.as<uint32_t>();
			rhs.height = height.as<uint32_t>();
			rhs.count = count.as<uint32_t>();
			return true;
		}
	};
}