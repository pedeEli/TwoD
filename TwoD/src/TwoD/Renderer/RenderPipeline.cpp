#include "tdpch.hpp"
#include "RenderPipeline.hpp"

#include "TwoD/Core/App.hpp"
#include "TwoD/Renderer/SpriteRenderer.hpp"
#include "TwoD/SDL/Window.hpp"

namespace TwoD
{
	void RenderPipeline::Render() const
	{
		auto& ecs = App::Get<ECS>();
		auto& window = App::Get<Window>();

		auto commandBuffer = window.AcquireCommandBuffer();
		auto renderPass = window.BeginRenderPass(&commandBuffer);

		if (!renderPass.Valid())
		{
			return;
		}

		for (auto& layer : m_layers)
		{
			if (layer->m_dirty)
			{
				layer->Update(ecs, window);
				layer->m_dirty = false;
			}
			layer->Render(ecs, commandBuffer, renderPass);
		}
	}
}