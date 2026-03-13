#include "tdpch.hpp"
#include "TextRenderHandler.hpp"

namespace TwoD
{
	void TextRenderHandler::Init()
	{
		m_shader = &AssetManager::Get<Shader>("TwoDLib::TextRenderer");
	}

	void TwoD::TextRenderHandler::Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const
	{
		m_shader->Bind(renderPass);
	}

	void TextRenderHandler::Render(Renderer& renderer, size_t index)
	{
		auto& textRenderer = GetComponents<TextRenderer>()[index];
		auto& transform = textRenderer.GetTransform()->GetWorldMatrix();
		auto& glyphs = textRenderer.GetGlyphs();

		for (auto& glyph : glyphs)
		{
			renderer.RenderQuad(
				transform,
				glyph.quadMin * textRenderer.size,
				glyph.quadSize * textRenderer.size,
				glyph.texMin,
				glyph.texMax,
				{ &textRenderer.font->binding, 1 }
			);
		}
	}
}