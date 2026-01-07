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
		if (m_raw)
		{
			SDL_ReleaseGPUTransferBuffer(m_raw->device, m_raw->buffer);
		}
	}

	void* TransferBuffer::Map(bool cycle) const
	{
		return SDL_MapGPUTransferBuffer(m_raw->device, m_raw->buffer, cycle);
	}

	TransferBuffer::TransferBuffer(TransferBuffer&& other) noexcept = default;
	TransferBuffer& TransferBuffer::operator=(TransferBuffer&& other) noexcept = default;
}