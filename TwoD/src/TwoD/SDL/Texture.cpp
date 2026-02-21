#include "tdpch.hpp"
#include "Texture.hpp"

#include "Window.hpp"
#include "Raw/Window.hpp"
#include "Raw/Texture.hpp"

namespace TwoD::SDL
{
    Texture::Texture() = default;
	Texture::Texture(const Window* window, const TextureInfo& info)
	{
		auto* device = window->m_raw->device;
        SDL_GPUTextureCreateInfo sdlInfo{
            .type = static_cast<SDL_GPUTextureType>(info.type),
            .format = static_cast<SDL_GPUTextureFormat>(info.format),
            .usage = static_cast<SDL_GPUTextureUsageFlags>(info.usage),
            .width = info.width,
            .height = info.height,
            .layer_count_or_depth = info.layerCountOrDepth,
            .num_levels = info.numLevels,
            .sample_count = static_cast<SDL_GPUSampleCount>(info.sampleCount),
        };
        auto* texture = SDL_CreateGPUTexture(device, &sdlInfo);
        m_raw = std::make_unique<Raw>(device, texture);
    }
    Texture::~Texture()
    {
		TD_CORE_ASSERT(!m_raw || m_released);
    }

	void Texture::Release()
	{
		TD_CORE_ASSERT(!m_released);
		m_released = true;
		if (m_raw)
		{
			SDL_ReleaseGPUTexture(m_raw->device, m_raw->texture);
		}
	}

	void Texture::swap(Texture&& other)
	{
		std::swap(m_raw, other.m_raw);
		std::swap(m_released, other.m_released);
	}

	Texture::Texture(Texture&& other) noexcept
	{
		swap(std::move(other));
	}
	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			swap(std::move(other));
		}
		return *this;
	}

    void Texture::SetName(const std::string& name)
    {
		TD_CORE_ASSERT(m_raw && !m_released);
        SDL_SetGPUTextureName(m_raw->device, m_raw->texture, name.c_str());
    }
}