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
		TD_CORE_ASSERT(!m_raw || m_released);
    }

	void Sampler::Release()
	{
		TD_CORE_ASSERT(!m_released);
		m_released = true;
		if (m_raw)
		{
			SDL_ReleaseGPUSampler(m_raw->device, m_raw->sampler);
		}
	}

	void Sampler::swap(Sampler&& other)
	{
		std::swap(m_raw, other.m_raw);
		std::swap(m_released, other.m_released);
	}

	Sampler::Sampler(Sampler&& other) noexcept
	{
		swap(std::move(other));
	}

	Sampler& Sampler::operator=(Sampler&& other) noexcept
	{
		if (this != &other)
		{
			swap(std::move(other));
		}
		return *this;
	}
}