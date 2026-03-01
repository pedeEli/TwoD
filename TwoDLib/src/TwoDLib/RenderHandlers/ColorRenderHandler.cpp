#include "tdpch.hpp"
#include "ColorRenderHandler.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"
#include "TwoDLib/Components/ScissorRect.hpp"

namespace TwoD
{
	void ColorRenderHandler::Init()
	{
		m_shader = &AssetManager::Get<Shader>("TwoDLib::ColorRenderer");
	}

	void TwoD::ColorRenderHandler::Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const
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
			size = rect->GetSize();
			pos = -size * 0.5f;
		}

		renderer.RenderQuad(
			transform->GetWorldMatrix(),
			pos,
			size,
			{
				static_cast<float>(colorRenderer.r) / 255.0f,
				static_cast<float>(colorRenderer.g) / 255.0f,
				static_cast<float>(colorRenderer.b) / 255.0f,
				static_cast<float>(colorRenderer.a) / 255.0f
			}
		);
	}

	void ColorRenderHandler::Update(size_t handlerIndex)
	{
		auto& renderers = GetComponents<ColorRenderer>();
		auto size = renderers.size();

		auto* camera = Camera::Get();
		m_rendererInfos.clear();
		m_rendererInfos.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			auto* rect = renderers[i].TryGetComponent<UITransform>();
			auto* scissor = renderers[i].TryGetComponent<ScissorRect>();
			m_rendererInfos.emplace_back(
				handlerIndex,
				i,
				renderers[i].layer,
				rect == nullptr
					? &camera->GetProjectionViewMatrix()
					: &camera->GetProjectionMatrixFixedZoom(),
				scissor == nullptr ? nullptr : &scissor->rect
			);
		}

		std::sort(m_rendererInfos.begin(), m_rendererInfos.end());
	}

	const std::vector<std::type_index>& ColorRenderHandler::GetRendererTypes() const
	{
		return s_types;
	}
}