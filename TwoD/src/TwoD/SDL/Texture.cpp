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
        if (m_raw)
        {
            SDL_ReleaseGPUTexture(m_raw->device, m_raw->texture);
        }
    }

    void Texture::SetName(const std::string& name)
    {
        SDL_SetGPUTextureName(m_raw->device, m_raw->texture, name.c_str());
    }

    Texture::Texture(Texture&& other) noexcept = default;
    Texture& Texture::operator=(Texture&& other) noexcept = default;
}