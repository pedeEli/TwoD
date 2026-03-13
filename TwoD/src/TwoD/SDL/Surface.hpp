#pragma once
#include "SDLDefines.hpp"

namespace TwoD::SDL
{
	class Surface
	{
	public:
		static Surface LoadFile(const std::filesystem::path& path);

	public:
		Surface();
		Surface(uint32_t width, uint32_t height, PixelFormat format);
		~Surface();
		Surface(const Surface& other) = delete;
		Surface(Surface&& other) noexcept;
		Surface& operator=(const Surface& other) = delete;
		Surface& operator=(Surface&& other) noexcept;

		void Destroy();
		void swap(Surface&& other);

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		void* GetPixels() const;

		void BlitTo(const Rect& srcRect, Surface& dest, const Rect& destRect) const;
		void BlitTo(Surface& dest, const Rect& destRect) const;

		void SaveBMP(const std::string& file) const;

	private:
		bool m_destroyed = false;
		struct Raw;
		std::unique_ptr<Raw> m_raw;
	};
}

