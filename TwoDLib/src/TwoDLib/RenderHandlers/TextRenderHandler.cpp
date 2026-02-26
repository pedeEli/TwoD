#include "tdpch.hpp"
#include "TextRenderHandler.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"
#include "TwoDLib/Components/UI/RectTransform.hpp"

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
		auto& transform = textRenderer.GetComponent<Transform>().GetWorldMatrix();
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

	void TextRenderHandler::Update(size_t handlerIndex)
	{
		auto& renderers = GetComponents<TextRenderer>();
		auto size = renderers.size();

		auto* camera = Camera::Get();
		m_rendererInfos.clear();
		m_rendererInfos.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			auto* rect = renderers[i].TryGetComponent<RectTransform>();
			m_rendererInfos.emplace_back(
				handlerIndex,
				i,
				renderers[i].layer,
				rect == nullptr
					? &camera->GetProjectionViewMatrix()
					: &camera->GetProjectionMatrixFixedZoom()
			);
		}

		std::sort(m_rendererInfos.begin(), m_rendererInfos.end());
	}

	const std::vector<std::type_index>& TextRenderHandler::GetRendererTypes() const
	{
		return s_types;
	}
}