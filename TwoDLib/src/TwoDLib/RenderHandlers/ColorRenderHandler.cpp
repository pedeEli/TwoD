#include "tdpch.hpp"
#include "ColorRenderHandler.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

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
		auto& transform = colorRenderer.GetComponent<Transform>();
		renderer.RenderQuad(
			transform.GetWorldMatrix(),
			{ -0.5f, -0.5f },
			{ 1.0f, 1.0f },
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
			m_rendererInfos.emplace_back(
				handlerIndex,
				i,
				renderers[i].layer,
				renderers[i].renderLocation == RenderLocation::InWorld
					? &camera->GetProjectionViewMatrix()
					: &camera->GetProjectionMatrixFixedZoom()
			);
		}

		std::sort(m_rendererInfos.begin(), m_rendererInfos.end());
	}

	const std::vector<std::type_index>& ColorRenderHandler::GetRendererTypes() const
	{
		return s_types;
	}
}