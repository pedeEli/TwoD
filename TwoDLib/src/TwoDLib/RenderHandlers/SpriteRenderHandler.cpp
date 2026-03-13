#include "tdpch.hpp"
#include "SpriteRenderHandler.hpp"

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
}