#include "tdpch.hpp"
#include "SpriteRenderLayer.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

namespace TwoD
{
	void SpriteRenderLayer::Init()
	{
		m_shader = &App::Get<AssetManager>().Get<Shader>("TwoDLib::SpriteRenderer");
		m_spriteAtlas = &App::Get<AssetManager>().Get<SpriteAtlas>("sprite-atlas");
		m_spriteAtlas->Pack();
	}

	void SpriteRenderLayer::Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const
	{
		m_shader->Bind(renderPass);
	}

	void SpriteRenderLayer::Render(const ECS& ecs, RenderSystem& renderSystem, SDL::RenderPass* renderPass, size_t index)
	{
		auto& renderer = ecs.GetComponents<SpriteRenderer>()[index];
		auto transform = renderer.GetComponent<Transform>();
		auto& rect = renderer.slice ? renderer.sprite->GetRect(*renderer.slice) : renderer.sprite->GetRect();
		renderSystem.RenderQuad(
			transform->GetWorldMatrix(),
			{ -0.5f, -0.5f },
			{ 1.0f, 1.0f },
			{ rect.u, rect.v },
			{ rect.u + rect.w, rect.v + rect.h },
			{ &m_spriteAtlas->binding, 0 }
		);
	}

	void SpriteRenderLayer::Update(const ECS& ecs)
	{
		auto& renderers = ecs.GetComponents<SpriteRenderer>();
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
					&camera->GetProjectionViewMatrix()
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

	const std::vector<std::type_index>& SpriteRenderLayer::GetRendererTypes() const
	{
		return s_types;
	}
}