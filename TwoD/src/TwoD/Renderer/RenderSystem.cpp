#include "tdpch.hpp"
#include "RenderSystem.hpp"

#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void RenderSystem::Init()
	{
		for (auto& renderLayer : m_renderHandlers)
		{
			renderLayer->Init();
		}

		m_renderer.Init();
	}

	void RenderSystem::Render()
	{
		auto& ecs = App::Get<ECS>();
		if (m_dirty)
		{
			Update(ecs);
			m_dirty = false;
		}

		m_renderer.Render(ecs, m_rendererHandlerInfos, &m_renderHandlers);
	}

	void RenderSystem::Update(const ECS& ecs)
	{
		size_t totalSize = 0;
		for (size_t i = 0; i < m_renderHandlers.size(); i++)
		{
			auto& handler = m_renderHandlers[i];
			handler->Update(ecs, i);
			totalSize += handler->m_rendererInfos.size();
		}

		m_rendererHandlerInfos.clear();
		m_rendererHandlerInfos.reserve(totalSize);
		std::vector<RendererHandlerInfo> temp;
		temp.reserve(totalSize);

		for (auto& handler : m_renderHandlers)
		{
			std::merge(
				handler->m_rendererInfos.begin(), handler->m_rendererInfos.end(),
				m_rendererHandlerInfos.begin(), m_rendererHandlerInfos.end(),
				std::back_inserter(temp)
			);
			std::swap(m_rendererHandlerInfos, temp);
			temp.clear();
		}
	}
}