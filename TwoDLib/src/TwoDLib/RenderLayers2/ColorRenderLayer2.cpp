#include "tdpch.hpp"
#include "ColorRenderLayer2.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

namespace TwoD
{
	void ColorRenderLayer2::Init()
	{
		m_shader = &App::Get<AssetManager>().Get<Shader>("TwoDLib::ColorRenderer2");
	}

	void ColorRenderLayer2::Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const
	{
		m_shader->Bind(renderPass);
	}

	void ColorRenderLayer2::Render(const ECS& ecs, RenderSystem2& renderSystem, SDL::RenderPass* renderPass, size_t index)
	{
		auto& renderer = ecs.GetComponents<ColorRenderer>()[index];
		auto transform = renderer.GetComponent<Transform>();
		renderSystem.RenderQuad(
			transform->GetWorldMatrix(),
			{ -0.5f, -0.5f },
			{ 1.0f, 1.0f },
			{
				static_cast<float>(renderer.r) / 255.0f,
				static_cast<float>(renderer.g) / 255.0f,
				static_cast<float>(renderer.b) / 255.0f,
				static_cast<float>(renderer.a) / 255.0f
			}
		);
	}

	void ColorRenderLayer2::Update(const ECS& ecs)
	{
		auto& renderers = ecs.GetComponents<ColorRenderer>();
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

	const std::vector<std::type_index>& ColorRenderLayer2::GetRendererTypes() const
	{
		return s_types;
	}
}