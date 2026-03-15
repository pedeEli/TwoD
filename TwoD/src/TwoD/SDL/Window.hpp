#pragma once
#include "TwoD/Debug/DebugDefines.hpp"
#include "TwoD/Events/EventHandler.hpp"

#include "SDLDefines.hpp"
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
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;

		bool Init(const WindowInfo& info);
		void ReleaseAndDestroy();

		glm::u32vec2 GetSize() const;
		uint32_t GetWindowID() const;
		SDL::ShaderFormat GetShaderFormats() const;
		SDL::TextureFormat GetSwapchainTextureFormat() const;
		void WaitForGPUIdle() const;

		SDL::Buffer CreateBuffer(const SDL::BufferInfo& info) const;
		SDL::TransferBuffer CreateTransferBuffer(const SDL::TransferBufferInfo& info) const;

		SDL::CommandBuffer AcquireCommandBuffer() const;
		SDL::RenderPass BeginRenderPass(SDL::CommandBuffer* commandBuffer, const SDL::Texture* targetTexture = nullptr) const;

		SDL::Shader CreateShader(const SDL::ShaderInfo& info) const;
		SDL::GraphicsPipeline CreateGraphicsPipeline(const SDL::GraphicsPipelineInfo& info) const;

		SDL::Texture CreateTexture(const SDL::TextureInfo& info) const;
		SDL::Sampler CreateSampler(const SDL::SamplerInfo& info) const;

	private:
		bool m_releasedAndDestroyed = false;

		EventHandler::Handle m_windowResizedHandle;
		glm::u32vec2 m_size = { 0, 0 };

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

