#include "tdpch.hpp"
#include "SpriteRenderHandler.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

namespace TwoD
{
	void SpriteRenderHandler::Init()
	{
		m_shader = &AssetManager::Get<Shader>("TwoDLib::SpriteRenderer");
		m_spriteAtlas = &AssetManager::Get<SpriteAtlas>("sprite-atlas");
		m_spriteAtlas->Pack();
	}

	void TwoD::SpriteRenderHandler::Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const
	{
        m_shader->Bind(renderPass);
	}

	void SpriteRenderHandler::Render(Renderer& renderer, size_t index)
	{
		auto& spriteRenderer = GetComponents<SpriteRenderer>()[index];
		auto* transform = spriteRenderer.GetTransform();
		auto& sprite = spriteRenderer.slice ? spriteRenderer.sprite->GetRect(*spriteRenderer.slice) : spriteRenderer.sprite->GetRect();
		
		glm::fvec2 pos = { -0.5f, -0.5f };
		glm::fvec2 size = { 1.0f, 1.0f };

		auto* rect = spriteRenderer.TryGetComponent<UITransform>();
		if (rect)
		{
			size = rect->GetSize();
			pos = -size * 0.5f;
		}

		renderer.RenderQuad(
			transform->GetWorldMatrix(),
			pos,
			size,
			{ sprite.u, sprite.v },
			{ sprite.u + sprite.w, sprite.v + sprite.h },
			{ &m_spriteAtlas->binding, 0 }
		);
	}

	void SpriteRenderHandler::Update(size_t handlerIndex)
	{
		auto& renderers = GetComponents<SpriteRenderer>();
		auto size = renderers.size();

		auto* camera = Camera::Get();
		m_rendererInfos.clear();
		m_rendererInfos.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			auto* rect = renderers[i].TryGetComponent<UITransform>();
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

	const std::vector<std::type_index>& SpriteRenderHandler::GetRendererTypes() const
	{
		return s_types;
	}
}