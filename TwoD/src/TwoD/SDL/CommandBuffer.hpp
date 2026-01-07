#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
	class CommandBuffer
	{
	public:
		CommandBuffer(const Window* window);
		~CommandBuffer();

		template<typename T>
		void PushVertexUniformData(uint32_t slotIndex, const T& data) const
		{
			PushVertexUniformData(slotIndex, &data, sizeof(T));
		}
		void PushVertexUniformData(uint32_t slotIndex, const void* data, uint32_t length) const;

		CopyPass BeginCopyPass() const;

	private:
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class RenderPass;
		friend class CopyPass;
	};
}

