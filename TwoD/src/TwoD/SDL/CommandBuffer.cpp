#include "tdpch.hpp"
#include "CommandBuffer.hpp"

#include "Window.hpp"
#include "CopyPass.hpp"
#include "Raw/Window.hpp"
#include "Raw/CommandBuffer.hpp"

namespace TwoD::SDL
{
	CommandBuffer::CommandBuffer(const Window* window)
	{
		auto* device = window->m_raw->device;
		auto* buffer = SDL_AcquireGPUCommandBuffer(device);
		m_raw = std::make_unique<Raw>(buffer);
	}
	CommandBuffer::~CommandBuffer()
	{
		SDL_SubmitGPUCommandBuffer(m_raw->buffer);
	}

	void CommandBuffer::PushVertexUniformData(uint32_t slotIndex, const void* data, uint32_t length) const
	{
		SDL_PushGPUVertexUniformData(m_raw->buffer, slotIndex, data, length);
	}
	
	CopyPass CommandBuffer::BeginCopyPass() const
	{
		return CopyPass(this);
	}
}