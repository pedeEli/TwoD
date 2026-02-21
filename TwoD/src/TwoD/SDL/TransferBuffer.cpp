#include "tdpch.hpp"
#include "TransferBuffer.hpp"

#include "Window.hpp"
#include "Raw/Window.hpp"
#include "Raw/TransferBuffer.hpp"

namespace TwoD::SDL
{
	TransferBuffer::TransferBuffer() = default;
	TransferBuffer::TransferBuffer(const Window* window, const TransferBufferInfo& info)
	{
		auto* device = window->m_raw->device;
		SDL_GPUTransferBufferCreateInfo sdlInfo{
			.usage = static_cast<SDL_GPUTransferBufferUsage>(info.usage),
			.size = info.size
		};
		auto* buffer = SDL_CreateGPUTransferBuffer(device, &sdlInfo);
		m_raw = std::make_unique<Raw>(device, buffer);
	}
	TransferBuffer::~TransferBuffer()
	{
		TD_CORE_ASSERT(!m_raw || m_released);
	}

	void TransferBuffer::Release()
	{
		TD_CORE_ASSERT(!m_released);
		m_released = true;
		if (m_raw)
		{
			SDL_ReleaseGPUTransferBuffer(m_raw->device, m_raw->buffer);
		}
	}

	void TransferBuffer::swap(TransferBuffer&& other)
	{
		std::swap(m_raw, other.m_raw);
		std::swap(m_released, other.m_released);
	}

	TransferBuffer::TransferBuffer(TransferBuffer&& other) noexcept
	{
		swap(std::move(other));
	}
	
	TransferBuffer& TransferBuffer::operator=(TransferBuffer&& other) noexcept
	{
		if (this != &other)
		{
			swap(std::move(other));
		}
		return *this;
	}

	void* TransferBuffer::Map(bool cycle) const
	{
		TD_CORE_ASSERT(m_raw && !m_released);
		return SDL_MapGPUTransferBuffer(m_raw->device, m_raw->buffer, cycle);
	}
}