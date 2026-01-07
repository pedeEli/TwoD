#include "tdpch.hpp"
#include "RenderSystem.hpp"

namespace TwoD
{
	void RenderSystem::SetPipeline(RenderPipeline& pipeline)
	{
		m_pipeline = std::move(pipeline);
	}

	void RenderSystem::Render() const
	{
		m_pipeline.Render();
	}
}