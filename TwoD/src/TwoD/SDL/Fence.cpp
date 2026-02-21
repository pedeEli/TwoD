#include "tdpch.hpp"
#include "Fence.hpp"

#include "Raw/Fence.hpp"
#include "Raw/CommandBuffer.hpp"

namespace TwoD::SDL
{
	Fence::Fence() = default;
	Fence::Fence(const CommandBuffer* commandBuffer)
	{
		TD_CORE_ASSERT(commandBuffer->m_raw && commandBuffer->m_submitted);
		auto* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(commandBuffer->m_raw->buffer);
		m_raw = std::make_unique<Raw>(fence, commandBuffer->m_raw->device);
	}
	Fence::~Fence()
	{
		TD_CORE_ASSERT(!m_raw || m_released);
	}

	void Fence::Release()
	{
		TD_CORE_ASSERT(!m_released);
		m_released = true;
		if (m_raw)
		{
			SDL_ReleaseGPUFence(m_raw->device, m_raw->fence);
		}
	}

	void Fence::Wait() const
	{
		TD_CORE_ASSERT(!m_released);
		if (m_raw)
		{
			SDL_WaitForGPUFences(m_raw->device, true, &m_raw->fence, 1);
		}
	}

	void Fence::swap(Fence&& other)
	{
		std::swap(m_raw, other.m_raw);
		std::swap(m_released, other.m_released);
	}

	Fence::Fence(Fence&& other) noexcept
	{
		swap(std::move(other));
	}

	Fence& Fence::operator=(Fence&& other) noexcept
	{
		if (this != &other)
		{
			swap(std::move(other));
		}
		return *this;
	}
}