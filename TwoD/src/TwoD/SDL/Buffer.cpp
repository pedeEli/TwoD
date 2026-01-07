#include "tdpch.hpp"
#include "Buffer.hpp"

#include "Window.hpp"
#include "Raw/Window.hpp"
#include "Raw/Buffer.hpp"

namespace TwoD::SDL
{
	Buffer::Buffer() = default;
	Buffer::Buffer(const Window* window, const BufferInfo& info)
	{
		auto* device = window->m_raw->device;
		SDL_GPUBufferCreateInfo sdlInfo{
			.usage = static_cast<SDL_GPUBufferUsageFlags>(info.usage),
			.size = info.size
		};
		auto* buffer = SDL_CreateGPUBuffer(device, &sdlInfo);
		if (!buffer)
		{
			TD_CORE_ERROR("Failed to create gpu buffer: {}", SDL_GetError());
		}
		m_raw = std::make_unique<Raw>(device, buffer);
	}

	Buffer::~Buffer()
	{
		if (m_raw)
		{
			SDL_ReleaseGPUBuffer(m_raw->device, m_raw->buffer);
		}
	}

	Buffer::Buffer(Buffer&& other) noexcept = default;
	Buffer& Buffer::operator=(Buffer&& other) noexcept = default;
}