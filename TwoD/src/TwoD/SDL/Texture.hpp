#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
	class Texture
	{
    public:
        Texture();
        Texture(const Window* window, const TextureInfo& info);
        ~Texture();
        Texture(const Texture& other) = delete;
        Texture(Texture&& other) noexcept;
        Texture& operator=(const Texture& other) = delete;
        Texture& operator=(Texture&& other) noexcept;

		void Release();
		void swap(Texture&& other);
        void SetName(const std::string& name);

    private:
		bool m_released = false;
        struct Raw;
        std::unique_ptr<Raw> m_raw;

        friend class RenderPass;
        friend class CopyPass;
		friend class CommandBuffer;
	};
}


