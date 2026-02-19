#include "tdpch.hpp"
#include "CommandBuffer.hpp"

#include "Window.hpp"
#include "CopyPass.hpp"
#include "Fence.hpp"
#include "Raw/Window.hpp"
#include "Raw/Texture.hpp"
#include "Raw/CommandBuffer.hpp"

namespace TwoD::SDL
{
	CommandBuffer::CommandBuffer(const Window* window)
	{
		auto* device = window->m_raw->device;
		auto* buffer = SDL_AcquireGPUCommandBuffer(device);
		m_raw = std::make_unique<Raw>(buffer, device);
	}
	CommandBuffer::~CommandBuffer()
	{
		if (!m_submitted)
		{
			SDL_CancelGPUCommandBuffer(m_raw->buffer);
		}
	}

	Fence CommandBuffer::Submit()
	{
		m_submitted = true;
		return Fence(this);
	}

	void CommandBuffer::PushVertexUniformData(uint32_t slotIndex, const void* data, uint32_t length) const
	{
		SDL_PushGPUVertexUniformData(m_raw->buffer, slotIndex, data, length);
	}
	
	CopyPass CommandBuffer::BeginCopyPass() const
	{
		return CopyPass(this);
	}

	void CommandBuffer::BlitTexture(const SDL::BlitInfo& blitInfo) const
	{
		SDL_GPUBlitInfo info{
			.source = {
				.texture = blitInfo.source.texture->m_raw->texture,
				.mip_level = blitInfo.source.mipLevel,
				.layer_or_depth_plane = blitInfo.source.layerOrDepthPlane,
				.x = blitInfo.source.x,
				.y = blitInfo.source.y,
				.w = blitInfo.source.w,
				.h = blitInfo.source.h,
			},
			.destination = {
				.texture = blitInfo.destination.texture->m_raw->texture,
				.mip_level = blitInfo.destination.mipLevel,
				.layer_or_depth_plane = blitInfo.destination.layerOrDepthPlane,
				.x = blitInfo.destination.x,
				.y = blitInfo.destination.y,
				.w = blitInfo.destination.w,
				.h = blitInfo.destination.h,
			},
			.load_op = static_cast<SDL_GPULoadOp>(blitInfo.loadOp),
			.clear_color = {
				.r = blitInfo.clearColor.r,
				.g = blitInfo.clearColor.g,
				.b = blitInfo.clearColor.b,
				.a = blitInfo.clearColor.a,
			},
			.flip_mode = static_cast<SDL_FlipMode>(blitInfo.flipMode),
			.filter = static_cast<SDL_GPUFilter>(blitInfo.filter),
			.cycle = blitInfo.cycle,
		};

		SDL_BlitGPUTexture(m_raw->buffer, &info);
	}
}