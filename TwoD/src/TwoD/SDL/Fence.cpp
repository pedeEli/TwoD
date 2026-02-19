#include "tdpch.hpp"
#include "Fence.hpp"

#include "Raw/Fence.hpp"
#include "Raw/CommandBuffer.hpp"

namespace TwoD::SDL
{
	Fence::Fence() = default;
	Fence::Fence(Fence&& other) = default;
	Fence& Fence::operator=(Fence&& other) noexcept = default;
	Fence::Fence(const CommandBuffer* commandBuffer)
	{
		auto* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(commandBuffer->m_raw->buffer);
		m_raw = std::make_unique<Raw>(fence, commandBuffer->m_raw->device);
	}
	Fence::~Fence()
	{
		if (m_raw)
		{
			SDL_ReleaseGPUFence(m_raw->device, m_raw->fence);
		}
	}

	void Fence::Wait() const
	{
		if (m_raw)
		{
			SDL_WaitForGPUFences(m_raw->device, true, &m_raw->fence, 1);
		}
	}
}