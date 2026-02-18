#pragma once
#include "TwoD/Debug/DebugDefines.hpp"

#include "Defines.hpp"
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
		
		void GetSize(int& width, int& height) const;
		uint32_t GetWindowID() const;
		SDL::ShaderFormat GetShaderFormats() const;
		SDL::TextureFormat GetSwapchainTextureFormat() const;

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
		SDL::RenderPass BeginRenderPass(SDL::CommandBuffer* commandBuffer, const SDL::Texture* targetTexture = nullptr) const
		{
			return SDL::RenderPass(this, commandBuffer, targetTexture);
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
		int m_width = 0;
		int m_height = 0;

		SDL::TextureInfo m_depthTextureInfo;
		SDL::Texture m_depthTexture;
		uint32_t m_windowID = 0;

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

		friend class Debug;
	};
}

