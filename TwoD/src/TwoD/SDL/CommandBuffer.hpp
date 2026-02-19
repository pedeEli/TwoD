#pragma once
#include "TwoD/Debug/DebugDefines.hpp"

#include "Defines.hpp"

namespace TwoD::SDL
{
	class CommandBuffer
	{
	public:
		CommandBuffer(const Window* window);
		~CommandBuffer();

		Fence Submit();

		template<typename T>
		void PushVertexUniformData(uint32_t slotIndex, const T& data) const
		{
			PushVertexUniformData(slotIndex, &data, sizeof(T));
		}
		void PushVertexUniformData(uint32_t slotIndex, const void* data, uint32_t length) const;
		void BlitTexture(const SDL::BlitInfo& blitInfo) const;

		CopyPass BeginCopyPass() const;

	private:
		bool m_submitted = false;
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class RenderPass;
		friend class CopyPass;
		friend class Fence;

		friend class ::TwoD::Debug;
	};
}

