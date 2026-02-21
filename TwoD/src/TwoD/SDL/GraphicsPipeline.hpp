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
		GraphicsPipeline(const GraphicsPipeline& other) = delete;
		GraphicsPipeline(GraphicsPipeline&& other) noexcept;
		GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;
		GraphicsPipeline& operator=(GraphicsPipeline&& other) noexcept;

		void Release();
		void swap(GraphicsPipeline&& other);

	private:
		bool m_released = false;
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class RenderPass;
	};
}

