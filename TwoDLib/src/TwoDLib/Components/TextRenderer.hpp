#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Assets/Font.hpp"

TD_COMPONENT(
	(TD_NAME(TextRenderer), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(int32_t, layer, TD_INIT(0)),
		TD_COMPONENT_FIELD(float, size, TD_INIT(0.0f)),
		TD_COMPONENT_FIELD(Font*, font, TD_INIT(nullptr)),
		TD_COMPONENT_FIELD(std::string, text, TD_INIT({}), TD_UPDATER(SetGlyphs()))
	)
)
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

		void SetText(const std::string& text);
		const std::vector<Glyph>& GetGlyphs() const;

		glm::fvec2 GetSize() const;

	private:
		void SetGlyphs();

	private:
		std::vector<Glyph> m_glyphs;
		glm::fvec2 m_size = { 0.0f, 0.0f };
	};
}