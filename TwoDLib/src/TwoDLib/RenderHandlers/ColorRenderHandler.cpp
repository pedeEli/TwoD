#include "tdpch.hpp"
#include "ColorRenderHandler.hpp"

namespace TwoD
{
	void ColorRenderHandler::Init()
	{
		m_shader = &AssetManager::Get<Shader>("TwoDLib::ColorRenderer");
	}

	void TwoD::ColorRenderHandler::Bind([[maybe_unused]] const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const
	{
		m_shader->Bind(renderPass);
	}

	void ColorRenderHandler::Render(Renderer& renderer, size_t index)
	{
		auto& colorRenderer = GetComponents<ColorRenderer>()[index];
		auto* transform = colorRenderer.GetTransform();
		glm::fvec2 pos = { -0.5f, -0.5f };
		glm::fvec2 size = { 1.0f, 1.0f };

		auto* rect = colorRenderer.TryGetComponent<UITransform>();
		if (rect)
		{
			size = rect->size;
			pos = -size * 0.5f;
		}

		renderer.RenderQuad(
			transform->GetWorldMatrix(),
			pos,
			size,
			static_cast<glm::fvec4>(colorRenderer.color) / 255.0f
		);
	}
}