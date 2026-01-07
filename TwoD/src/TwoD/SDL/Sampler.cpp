#include "tdpch.hpp"
#include "Sampler.hpp"

#include "Window.hpp"
#include "Raw/Window.hpp"
#include "Raw/Sampler.hpp"

namespace TwoD::SDL
{
    Sampler::Sampler() = default;
	Sampler::Sampler(const Window* window, const SamplerInfo& info)
	{
        auto* device = window->m_raw->device;
        SDL_GPUSamplerCreateInfo sdlInfo{
            .min_filter = static_cast<SDL_GPUFilter>(info.minFilter),
            .mag_filter = static_cast<SDL_GPUFilter>(info.magFilter),
            .mipmap_mode = static_cast<SDL_GPUSamplerMipmapMode>(info.mipmapMode),
            .address_mode_u = static_cast<SDL_GPUSamplerAddressMode>(info.addressModeU),
            .address_mode_v = static_cast<SDL_GPUSamplerAddressMode>(info.addressModeV),
            .address_mode_w = static_cast<SDL_GPUSamplerAddressMode>(info.addressModeW),
            .mip_lod_bias = info.mipLodBias,
            .max_anisotropy = info.maxAnisotropy,
            .compare_op = static_cast<SDL_GPUCompareOp>(info.compareOp),
            .min_lod = info.minLod,
            .max_lod = info.maxLod,
            .enable_anisotropy = info.enableAnisotropy,
            .enable_compare = info.enableCompare
        };
        auto* sampler = SDL_CreateGPUSampler(device, &sdlInfo);
        m_raw = std::make_unique<Raw>(device, sampler);
	}
    Sampler::~Sampler()
    {
        if (m_raw)
        {
            SDL_ReleaseGPUSampler(m_raw->device, m_raw->sampler);
        }
    }

    Sampler::Sampler(Sampler&& other) noexcept = default;
    Sampler& Sampler::operator=(Sampler&& other) noexcept = default;
}