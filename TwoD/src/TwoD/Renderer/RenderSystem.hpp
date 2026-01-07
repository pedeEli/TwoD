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

	private:
		RenderPipeline m_pipeline;
	};
}