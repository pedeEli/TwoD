#pragma once
#include "Buffer.hpp"
#include "TransferBuffer.hpp"
#include "CommandBuffer.hpp"
#include "Shader.hpp"
#include "GraphicsPipeline.hpp"
#include "Texture.hpp"
#include "Sampler.hpp"
#include "RenderPass.hpp"
#include "CopyPass.hpp"

namespace TwoD
{
	enum class WindowFlags : uint64_t
	{
		FULLSCREEN          = 0x0000000000000001ULL,
		OPENGL              = 0x0000000000000002ULL,
		OCCLUDED            = 0x0000000000000004ULL,
		HIDDEN              = 0x0000000000000008ULL,
		BORDERLESS          = 0x0000000000000010ULL,
		RESIZABLE           = 0x0000000000000020ULL,
		MINIMIZED           = 0x0000000000000040ULL,
		MAXIMIZED           = 0x0000000000000080ULL,
		MOUSE_GRABBED       = 0x0000000000000100ULL,
		INPUT_FOCUS         = 0x0000000000000200ULL,
		MOUSE_FOCUS         = 0x0000000000000400ULL,
		EXTERNAL            = 0x0000000000000800ULL,
		MODAL               = 0x0000000000001000ULL,
		HIGH_PIXEL_DENSITY  = 0x0000000000002000ULL,
		MOUSE_CAPTURE       = 0x0000000000004000ULL,
		MOUSE_RELATIVE_MODE = 0x0000000000008000ULL,
		ALWAYS_ON_TOP       = 0x0000000000010000ULL,
		UTILITY             = 0x0000000000020000ULL,
		TOOLTIP             = 0x0000000000040000ULL,
		POPUP_MENU          = 0x0000000000080000ULL,
		KEYBOARD_GRABBED    = 0x0000000000100000ULL,
		VULKAN              = 0x0000000010000000ULL,
		METAL               = 0x0000000020000000ULL,
		//TRANSPARENT         = 0x0000000040000000ULL,
		NOT_FOCUSABLE       = 0x0000000080000000ULL
	};

	constexpr WindowFlags operator|(WindowFlags a, WindowFlags b) noexcept
	{
		return static_cast<WindowFlags>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
	}

	struct WindowInfo
	{
		int width;
		int height;
		std::string title;
		WindowFlags windowFlags;
	};

	class Window
	{
	public:
		Window();
		~Window();
		Window(Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(Window& other) = delete;
		Window& operator=(Window&& other) = delete;

		bool Init(const WindowInfo& info);
		
		void SetSize(int width, int height);
		void GetSize(int& width, int& height) const;
		SDL::ShaderFormat GetShaderFormats();
		SDL::TextureFormat GetSwapchainTextureFormat();

		SDL::Buffer CreateBuffer(const SDL::BufferInfo& info) const
		{
			return SDL::Buffer(this, info);
		}
		SDL::TransferBuffer CreateTransferBuffer(const SDL::TransferBufferInfo& info) const
		{
			return SDL::TransferBuffer(this, info);
		}

		SDL::CommandBuffer AcquireCommandBuffer() const
		{
			return SDL::CommandBuffer(this);
		}
		SDL::RenderPass BeginRenderPass(SDL::CommandBuffer* commandBuffer) const
		{
			return SDL::RenderPass(this, commandBuffer);
		}

		SDL::Shader CreateShader(const SDL::ShaderInfo& info) const
		{
			return SDL::Shader(this, info);
		}
		SDL::GraphicsPipeline CreateGraphicsPipeline(const SDL::GraphicsPipelineInfo& info) const
		{
			return SDL::GraphicsPipeline(this, info);
		}

		SDL::Texture CreateTexture(const SDL::TextureInfo& info) const
		{
			return SDL::Texture(this, info);
		}
		SDL::Sampler CreateSampler(const SDL::SamplerInfo& info) const
		{
			return SDL::Sampler(this, info);
		}

	private:
		int m_width;
		int m_height;

		SDL::TextureInfo m_depthTextureInfo;
		SDL::Texture m_depthTexture;

		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class SDL::Buffer;
		friend class SDL::TransferBuffer;
		friend class SDL::CommandBuffer;
		friend class SDL::Shader;
		friend class SDL::GraphicsPipeline;
		friend class SDL::Texture;
		friend class SDL::Sampler;
		friend class SDL::RenderPass;
	};
}

