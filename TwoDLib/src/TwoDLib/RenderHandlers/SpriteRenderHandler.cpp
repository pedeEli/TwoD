#include "tdpch.hpp"
#include "SpriteRenderHandler.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

namespace TwoD
{
	void SpriteRenderHandler::Init()
	{
		m_shader = &App::Get<AssetManager>().Get<Shader>("TwoDLib::SpriteRenderer");
		m_spriteAtlas = &App::Get<AssetManager>().Get<SpriteAtlas>("sprite-atlas");
		m_spriteAtlas->Pack();
	}

	void SpriteRenderHandler::Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const
	{
		m_shader->Bind(renderPass);
	}

	void SpriteRenderHandler::Render(Renderer& renderer, size_t index)
	{
		auto& spriteRenderer = GetComponents<SpriteRenderer>()[index];
		auto& transform = spriteRenderer.GetComponent<Transform>();
		auto& rect = spriteRenderer.slice ? spriteRenderer.sprite->GetRect(*spriteRenderer.slice) : spriteRenderer.sprite->GetRect();
		renderer.RenderQuad(
			transform.GetWorldMatrix(),
			{ -0.5f, -0.5f },
			{ 1.0f, 1.0f },
			{ rect.u, rect.v },
			{ rect.u + rect.w, rect.v + rect.h },
			{ &m_spriteAtlas->binding, 0 }
		);
	}

	void SpriteRenderHandler::Update(size_t handlerIndex)
	{
		auto& renderers = GetComponents<SpriteRenderer>();
		auto size = renderers.size();

		if (m_rendererInfos.size() != size)
		{
			auto* camera = Camera::Get();
			m_rendererInfos.clear();
			m_rendererInfos.reserve(size);
			for (size_t i = 0; i < size; i++)
			{
				m_rendererInfos.emplace_back(
					handlerIndex,
					i,
					renderers[i].layer,
					&camera->GetProjectionViewMatrix()
				);
			}
		}

		std::sort(m_rendererInfos.begin(), m_rendererInfos.end());
	}

	const std::vector<std::type_index>& SpriteRenderHandler::GetRendererTypes() const
	{
		return s_types;
	}
}