#pragma once

#include "TwoD/ECS/ECS.hpp"
#include "RenderPipeline.hpp"

namespace TwoD
{
	class RenderSystem
	{
	public:
		void Render() const;
		void SetPipeline(RenderPipeline& pipeline);
		
		template<class Renderer>
		requires(std::is_base_of_v<Component, Renderer>)
		void SetDirty()
		{
			m_pipeline.SetDirty<Renderer>();
		}

		template<class Layer>
		requires(std::is_base_of_v<RenderLayer, Layer>)
		Layer& GetLayer() const
		{
			return m_pipeline.GetLayer<Layer>();
		}

		template<class Layer>
		requires(std::is_base_of_v<RenderLayer, Layer>)
		void RegisterLayer()
		{
			std::string name = typeid(Layer).name();
			TD_CORE_ASSERT(!m_renderLayers.contains(name), "Cannot register RenderLayer twice!");
			m_renderLayers[name] = [](RenderPipeline& pipeline)
				{
					pipeline.AddLayer<Layer>();
				};
		}

		std::function<void(RenderPipeline&)> GetLayerAdder(const std::string& layer)
		{
			TD_CORE_ASSERT(m_renderLayers.contains(layer));
			return m_renderLayers[layer];
		}


	private:
		RenderPipeline m_pipeline;
		std::unordered_map<std::string, std::function<void(RenderPipeline&)>> m_renderLayers;
	};
}