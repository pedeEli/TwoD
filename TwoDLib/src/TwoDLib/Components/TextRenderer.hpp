#pragma once

#include "TwoD/ECS/ECS.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Assets/Font.hpp"

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

		void SetLayer(uint32_t layer);

		void SetText(const std::string& text);
		const std::vector<Glyph>& GetGlyphs() const;

	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD_WITH_UPDATER(int32_t, layer, 0, SetLayer(layer);),
			TD_COMPONENT_FIELD(Font*, font),
			TD_COMPONENT_FIELD_WITH_UPDATER(std::string, text, SetGlyphs();)
		)

	private:
		void SetGlyphs();

	private:
		std::vector<Glyph> m_glyphs;
	};
}