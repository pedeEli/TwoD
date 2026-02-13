#pragma once

#include "TwoD/ECS/ECS.hpp"
#include "TwoDLib/Assets/Font.hpp"
#include "TwoDLib/Defines.hpp"

namespace TwoD
{
	class TextRenderer : public Component
	{
		using Component::Component;
	public:
		struct Glyph
		{
			glm::fvec2 texMin;
			glm::fvec2 texMax;
			glm::fvec2 quadMin;
			glm::fvec2 quadSize;
		};

	public:
		void StartBefore() override;
		void Destroy() override;

		void SetText(const std::string& text);
		const std::vector<Glyph>& GetGlyphs() const;

	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD(int32_t, layer, 0),
			TD_COMPONENT_FIELD(Font*, font),
			TD_COMPONENT_FIELD(std::string, text),
			TD_COMPONENT_FIELD(RenderLocation, renderLocation, RenderLocation::InWorld)
		)

	private:
		void SetGlyphs();

	private:
		std::vector<Glyph> m_glyphs;
	};
}