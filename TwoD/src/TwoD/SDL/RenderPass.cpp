#include "tdpch.hpp"
#include "RenderPass.hpp"

#include "Window.hpp"
#include "Raw/Window.hpp"
#include "Raw/RenderPass.hpp"
#include "Raw/CommandBuffer.hpp"
#include "Raw/Texture.hpp"
#include "Raw/Sampler.hpp"
#include "Raw/Buffer.hpp"
#include "Raw/GraphicsPipeline.hpp"

namespace TwoD::SDL
{
	RenderPass::RenderPass(const Window* window, CommandBuffer* commandBuffer, const SDL::Texture* targetTexture)
	{
		SDL_GPUTexture* swapchainTexture = nullptr;
		if (targetTexture)
		{
			swapchainTexture = targetTexture->m_raw->texture;
		}
		else if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer->m_raw->buffer, window->m_raw->window, &swapchainTexture, nullptr, nullptr))
		{
			TD_CORE_WARN("Failed to acquaire gpu swapchain texture: {}", SDL_GetError());
			return;
		}

		if (!swapchainTexture)
		{
			return;
		}

		SDL_GPUColorTargetInfo colorTargetInfo{
			.texture = swapchainTexture,
			.clear_color = { 0.0f, 0.0f, 0.0f, 0.0f },
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_STORE
		};
		SDL_GPUDepthStencilTargetInfo depthTargetInfo{
			.texture = window->m_depthTexture.m_raw->texture,
			.clear_depth = 1.0f,
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_DONT_CARE
		};

		auto* renderPass = SDL_BeginGPURenderPass(commandBuffer->m_raw->buffer, &colorTargetInfo, 1, nullptr);
		m_raw = std::make_unique<Raw>(renderPass);
	}
	RenderPass::~RenderPass()
	{
		TD_CORE_ASSERT(!m_raw || m_ended);
	}

	void RenderPass::End()
	{
		TD_CORE_ASSERT(!m_ended);
		m_ended = true;
		if (m_raw)
		{
			SDL_EndGPURenderPass(m_raw->renderPass);
		}
	}

	void RenderPass::BindGraphicsPipeline(const GraphicsPipeline* pipeline) const
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		TD_CORE_ASSERT(pipeline->m_raw && !pipeline->m_released);
		SDL_BindGPUGraphicsPipeline(m_raw->renderPass, pipeline->m_raw->pipeline);
	}

	void RenderPass::BindFragmentSamplers(uint32_t firstSlot, const std::vector<TextureSamplerBinding>& bindings) const
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		std::vector<SDL_GPUTextureSamplerBinding> sdlBindings(bindings.size());
		for (size_t i = 0; i < bindings.size(); i++)
		{
			TD_CORE_ASSERT(bindings[i].sampler->m_raw && !bindings[i].sampler->m_released);
			TD_CORE_ASSERT(bindings[i].texture->m_raw && !bindings[i].texture->m_released);
			sdlBindings[i].texture = bindings[i].texture->m_raw->texture;
			sdlBindings[i].sampler = bindings[i].sampler->m_raw->sampler;
		}
		SDL_BindGPUFragmentSamplers(m_raw->renderPass, firstSlot, sdlBindings.data(), static_cast<uint32_t>(sdlBindings.size()));
	}
	void RenderPass::BindVertexStorageBuffers(uint32_t firstSlot, const std::vector<const Buffer*>& buffers) const
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		std::vector<SDL_GPUBuffer*> sdlBuffers(buffers.size());
		for (size_t i = 0; i < buffers.size(); i++)
		{
			TD_CORE_ASSERT(buffers[i]->m_raw && !buffers[i]->m_released);
			sdlBuffers[i] = buffers[i]->m_raw->buffer;
		}
		SDL_BindGPUVertexStorageBuffers(m_raw->renderPass, firstSlot, sdlBuffers.data(), static_cast<uint32_t>(sdlBuffers.size()));
	}

	void RenderPass::BindVertexBuffers(uint32_t firstSlot, const std::vector<BufferBinding> bindings) const
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		std::vector<SDL_GPUBufferBinding> sdlBindings(bindings.size());
		for (size_t i = 0; i < bindings.size(); i++)
		{
			TD_CORE_ASSERT(bindings[i].buffer->m_raw && !bindings[i].buffer->m_released);
			sdlBindings[i].buffer = bindings[i].buffer->m_raw->buffer;
			sdlBindings[i].offset = bindings[i].offset;
		}
		SDL_BindGPUVertexBuffers(m_raw->renderPass, firstSlot, sdlBindings.data(), static_cast<uint32_t>(sdlBindings.size()));
	}
	void RenderPass::BindIndexBuffer(const BufferBinding& binding, IndexElementSize indexElementSize) const
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		TD_CORE_ASSERT(binding.buffer->m_raw && !binding.buffer->m_released);
		SDL_GPUBufferBinding indexBindings{
			.buffer = binding.buffer->m_raw->buffer,
			.offset = binding.offset
		};
		SDL_BindGPUIndexBuffer(m_raw->renderPass, &indexBindings, static_cast<SDL_GPUIndexElementSize>(indexElementSize));
	}
	void RenderPass::DrawIndexedPrimitives(uint32_t numIndices, uint32_t numInstances, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		SDL_DrawGPUIndexedPrimitives(m_raw->renderPass, numIndices, numInstances, firstIndex, vertexOffset, firstInstance);
	}
	void RenderPass::DrawPrimitives(uint32_t numVertices, uint32_t numInstances, uint32_t firstVertex, uint32_t firstInstance) const
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		SDL_DrawGPUPrimitives(m_raw->renderPass, numVertices, numInstances, firstVertex, firstInstance);
	}

	void RenderPass::SetScissorRect(const std::optional<::TwoD::Rect<int>> rect) const
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		if (!rect)
		{
			SDL_SetGPUScissor(m_raw->renderPass, nullptr);
		}
		else
		{
			SDL_Rect r{
				.x = rect->min.x,
				.y = rect->min.y,
				.w = rect->max.x - rect->min.x,
				.h = rect->max.y - rect->min.y
			};
			SDL_SetGPUScissor(m_raw->renderPass, &r);
		}
	}

	RenderPass::operator bool() const noexcept
	{
		return static_cast<bool>(m_raw);
	}
}