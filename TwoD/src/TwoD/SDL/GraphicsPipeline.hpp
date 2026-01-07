#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline();
		GraphicsPipeline(const Window* window, const GraphicsPipelineInfo& info);
		~GraphicsPipeline();
		GraphicsPipeline(GraphicsPipeline&& other) noexcept;
		GraphicsPipeline& operator=(GraphicsPipeline&& other) noexcept;

	private:
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class RenderPass;
	};
}

