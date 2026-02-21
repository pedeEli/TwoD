#include "tdpch.hpp"
#include "CopyPass.hpp"

#include "Raw/TransferBuffer.hpp"
#include "Raw/CommandBuffer.hpp"
#include "Raw/CopyPass.hpp"
#include "Raw/Texture.hpp"
#include "Raw/Buffer.hpp"

namespace TwoD::SDL
{
    CopyPass::CopyPass(const CommandBuffer* commandBuffer)
    {
        auto* copyPass = SDL_BeginGPUCopyPass(commandBuffer->m_raw->buffer);
        m_raw = std::make_unique<Raw>(copyPass);
    }
    CopyPass::~CopyPass()
    {
		TD_CORE_ASSERT(m_raw && m_ended);
    }

	void CopyPass::End()
	{
		TD_CORE_ASSERT(m_raw && !m_ended);
		m_ended = true;
		SDL_EndGPUCopyPass(m_raw->copyPass);
	}

    void CopyPass::UploadToTexture(const TextureTransferInfo& source, const TextureRegion& destination, bool cycle) const
    {
		TD_CORE_ASSERT(m_raw && !m_ended);
		TD_CORE_ASSERT(source.transferBuffer->m_raw && !source.transferBuffer->m_released);
		TD_CORE_ASSERT(destination.texture->m_raw && !destination.texture->m_released);
        SDL_GPUTextureTransferInfo sdlSource{
            .transfer_buffer = source.transferBuffer->m_raw->buffer,
            .offset = source.offset,
            .pixels_per_row = source.pixelsPerRow,
            .rows_per_layer = source.rowsPerLayer
        };
        SDL_GPUTextureRegion sdlDestination{
            .texture = destination.texture->m_raw->texture,
            .mip_level = destination.mipLevel,
            .layer = destination.layer,
            .x = destination.x,
            .y = destination.y,
            .z = destination.z,
            .w = destination.w,
            .h = destination.h,
            .d = destination.d
        };
        SDL_UploadToGPUTexture(m_raw->copyPass, &sdlSource, &sdlDestination, cycle);
    }
    void CopyPass::UploadToBuffer(const TransferBufferLocation& source, const BufferRegion& destination, bool cycle) const
    {
		TD_CORE_ASSERT(m_raw && !m_ended);
		TD_CORE_ASSERT(source.transferBuffer->m_raw && !source.transferBuffer->m_released);
		TD_CORE_ASSERT(destination.buffer->m_raw && !destination.buffer->m_released);
        SDL_GPUTransferBufferLocation sdlSource{
            .transfer_buffer = source.transferBuffer->m_raw->buffer,
            .offset = source.offset
        };
        SDL_GPUBufferRegion sdlDestination{
            .buffer = destination.buffer->m_raw->buffer,
            .offset = destination.offset,
            .size = destination.size
        };
        SDL_UploadToGPUBuffer(m_raw->copyPass, &sdlSource, &sdlDestination, cycle);
    }
}