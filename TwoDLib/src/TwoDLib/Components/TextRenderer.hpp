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
			glm::fvec2 quadMax;
		};

	public:
		void StartBefore() override;
		void Destroy() override;

		void SetText(const std::string& text);
		const std::vector<Glyph>& GetGlyphs() const;

	public:
		TD_COMPONENT_FIELDS(
			TD_FIELD(int32_t, layer, 0),
			TD_FIELD(Font*, font),
			TD_FIELD(std::string, text),
			TD_FIELD(RenderLocation, renderLocation, RenderLocation::InWorld)
		)

	private:
		void SetGlyphs();

	private:
		std::vector<Glyph> m_glyphs;
	};
}