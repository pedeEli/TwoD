#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
	class RenderPass
	{
	public:
		RenderPass(const Window* window, CommandBuffer* commandBuffer);
		~RenderPass();

		void BindGraphicsPipeline(const GraphicsPipeline* pipeline) const;

		void BindFragmentSamplers(uint32_t firstSlot, const std::vector<TextureSamplerBinding>& bindings) const;
		void BindVertexStorageBuffers(uint32_t firstSlot, const std::vector<const Buffer*>& buffers) const;

		void BindVertexBuffers(uint32_t firstSlot, const std::vector<BufferBinding> bindings) const;
		void BindIndexBuffer(const BufferBinding& binding, IndexElementSize indexElementSize) const;
		void DrawIndexedPrimitives(uint32_t numIndices, uint32_t numInstances, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const;
		void DrawPrimitives(uint32_t numVertices, uint32_t numInstances, uint32_t firstVertex, uint32_t firstInstance) const;

		bool Valid();

	private:
		struct Raw;
		std::unique_ptr<Raw> m_raw;
	};
}

