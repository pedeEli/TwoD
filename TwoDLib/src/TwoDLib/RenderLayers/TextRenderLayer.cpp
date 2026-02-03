#include "tdpch.hpp"
#include "TextRenderLayer.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

namespace TwoD
{
	void TextRenderLayer::Init()
	{
		m_shader = &App::Get<AssetManager>().Get<Shader>("TwoDLib::TextRenderer");
	}

	void TextRenderLayer::Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const
	{
		m_shader->Bind(renderPass);
	}

	void TextRenderLayer::Render(const ECS& ecs, RenderSystem& renderSystem, SDL::RenderPass* renderPass, size_t index)
	{
		auto& renderer = ecs.GetComponents<TextRenderer>()[index];
		auto& transform = renderer.GetComponent<Transform>()->GetWorldMatrix();
		auto& glyphs = renderer.GetGlyphs();

		for (auto& glyph : glyphs)
		{
			renderSystem.RenderQuad(
				transform,
				glyph.quadMin,
				glyph.quadSize,
				glyph.texMin,
				glyph.texMax,
				{ &renderer.font->binding, 1 }
			);
		}
	}

	void TextRenderLayer::Update(const ECS& ecs)
	{
		auto& renderers = ecs.GetComponents<TextRenderer>();
		auto size = renderers.size();

		if (m_indexLayers.size() != size)
		{
			auto* camera = Camera::Get();
			m_indexLayers.clear();
			m_indexLayers.reserve(size);
			for (size_t i = 0; i < size; i++)
			{
				m_indexLayers.emplace_back(
					i,
					renderers[i].layer,
					renderers[i].renderLocation == RenderLocation::InWorld
					? &camera->GetProjectionViewMatrix()
					: &camera->GetProjectionMatrixFixedZoom()
				);
			}
		}

		std::sort(m_indexLayers.begin(), m_indexLayers.end(), [&renderers](IndexLayer& a, IndexLayer& b)
			{
				if (a.layer == b.layer)
				{
					return a.projection < b.projection;
				}
				return a.layer < b.layer;
			});
	}

	const std::vector<std::type_index>& TextRenderLayer::GetRendererTypes() const
	{
		return s_types;
	}
}