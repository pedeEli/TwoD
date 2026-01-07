#include "tdpch.hpp"
#include "Surface.hpp"
#include <SDL3_image/SDL_image.h>

#include "Raw/Surface.hpp"

namespace TwoD::SDL
{
	Surface Surface::LoadFile(const std::filesystem::path& path)
	{
		auto* imageData = IMG_Load(path.string().c_str());
		TD_CORE_ASSERT(imageData, std::format("Failed to load image: {}", SDL_GetError()))

		Surface surface;
		surface.m_raw = std::make_unique<Raw>(imageData);
		return surface;
	}

	Surface::Surface() {}
	Surface::Surface(uint32_t width, uint32_t height, PixelFormat format)
	{
		auto* surface = SDL_CreateSurface(width, height, static_cast<SDL_PixelFormat>(format));
		m_raw = std::make_unique<Raw>(surface);
	}
	Surface::~Surface()
	{
		if (m_raw)
		{
			SDL_DestroySurface(m_raw->surface);
		}
	}

	uint32_t Surface::GetWidth() const
	{
		return static_cast<uint32_t>(m_raw->surface->w);
	}
	uint32_t Surface::GetHeight() const
	{
		return static_cast<uint32_t>(m_raw->surface->h);
	}
	void* Surface::GetPixels() const
	{
		return m_raw->surface->pixels;
	}

	void Surface::BlitTo(const Rect& srcRect, Surface& dest, const Rect& destRect) const
	{
		SDL_Rect sdlSrcRect{ static_cast<int>(srcRect.x), static_cast<int>(srcRect.y), static_cast<int>(srcRect.w), static_cast<int>(srcRect.h) };
		SDL_Rect sdlDestRect{ static_cast<int>(destRect.x), static_cast<int>(destRect.y), static_cast<int>(destRect.w), static_cast<int>(destRect.h) };
		SDL_BlitSurface(m_raw->surface, &sdlSrcRect, dest.m_raw->surface, &sdlDestRect);
	}
	void Surface::BlitTo(Surface& dest, const Rect& destRect) const
	{
		SDL_Rect sdlDestRect{ static_cast<int>(destRect.x), static_cast<int>(destRect.y), static_cast<int>(destRect.w), static_cast<int>(destRect.h) };
		SDL_BlitSurface(m_raw->surface, nullptr, dest.m_raw->surface, &sdlDestRect);
	}

	void Surface::SaveBMP(const std::string& file) const
	{
		SDL_SaveBMP(m_raw->surface, file.c_str());
	}

	Surface::Surface(Surface&& other) noexcept = default;
	Surface& Surface::operator=(Surface&& other) noexcept = default;
}