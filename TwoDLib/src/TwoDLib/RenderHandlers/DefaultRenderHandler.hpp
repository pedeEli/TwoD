#pragma once
#include <concepts>
#include "TwoD.hpp"
#include "TwoDLib/Components/Camera.hpp"
#include "TwoDLib/Components/ScissorRect.hpp"

namespace TwoD
{
	template<class T>
	requires(std::is_base_of_v<Component, T>)
	class DefaultRenderHandler : public RenderHandler
	{
		void Update(size_t handlerIndex) override
		{
			auto& renderers = GetComponents<T>();
			auto size = renderers.size();

			auto* camera = Camera::Get();
			m_rendererInfos.clear();
			m_rendererInfos.reserve(size);
			for (size_t i = 0; i < size; i++)
			{
				auto* uiTransform = renderers[i].TryGetComponent<UITransform>();
				auto& projection = uiTransform ? camera->GetProjectionMatrixFixedZoom() : camera->GetProjectionViewMatrix();
				std::optional<Rect<float>> scissorRect;

				ComputedScissorRect* scissor = nullptr;
				if (scissor = renderers[i].TryGetComponent<ComputedScissorRect>())
				{
					if (uiTransform)
					{
						scissorRect = scissor->rect;
					}
					else
					{
						scissorRect = {
							camera->WorldToScreenSpace(scissor->rect.min),
							camera->WorldToScreenSpace(scissor->rect.max)
						};
					}
				}

				m_rendererInfos.emplace_back(
					handlerIndex,
					i,
					renderers[i].layer,
					&projection,
					scissorRect
				);
			}
		}
	};
}