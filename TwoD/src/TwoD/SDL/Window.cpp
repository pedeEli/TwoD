#include "tdpch.hpp"
#include "Window.hpp"

#include "Raw/Window.hpp"
#include "TwoD/Events/EventHandler.hpp"

namespace TwoD
{
	Window::Window() = default;
	Window::~Window()
	{
		TD_CORE_ASSERT(!m_raw || m_releasedAndDestroyed);
	}

	void Window::ReleaseAndDestroy()
	{
		TD_CORE_ASSERT(!m_releasedAndDestroyed);
		m_releasedAndDestroyed = true;
		if (m_raw)
		{
			SDL_Window* window = m_raw->window;
			SDL_GPUDevice* device = m_raw->device;
			if (window && device)
			{
				SDL_ReleaseWindowFromGPUDevice(device, window);
			}
			if (window)
			{
				SDL_DestroyWindow(window);
				m_raw->window = nullptr;
			}
			if (device)
			{
				SDL_DestroyGPUDevice(device);
				m_raw->device = nullptr;
			}
			m_depthTexture.Release();
		}
	}

	bool Window::Init(const WindowInfo& info)
	{
		auto* window = SDL_CreateWindow(
			info.title.data(),
			info.width, info.height,
			static_cast<SDL_WindowFlags>(info.windowFlags)
		);
		if (!window)
		{
			TD_CORE_CRITICAL("Failed to create window: {}", SDL_GetError());
			return false;
		}

		auto* device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, "vulkan");
		if (!device)
		{
			TD_CORE_CRITICAL("Failed to create gpu device: {}", SDL_GetError());
			return false;
		}

		if (!SDL_ClaimWindowForGPUDevice(device, window))
		{
			TD_CORE_CRITICAL("Failed to claim window for gpu device: {}", SDL_GetError());
			return false;
		}

		if (!SDL_GetWindowSize(window, (int*)&m_size.x, (int*)&m_size.y))
		{
			TD_CORE_CRITICAL("Failed to get window size: {}", SDL_GetError());
		}

		m_windowID = SDL_GetWindowID(window);

		m_raw = std::make_unique<Raw>(window, device);

		m_depthTextureInfo.type = SDL::TextureType::TWO_D;
		m_depthTextureInfo.width = m_size.x;
		m_depthTextureInfo.height = m_size.y;
		m_depthTextureInfo.layerCountOrDepth = 1;
		m_depthTextureInfo.numLevels = 1;
		m_depthTextureInfo.sampleCount = SDL::SampleCount::ONE;
		m_depthTextureInfo.format = SDL::TextureFormat::D16_UNORM;
		m_depthTextureInfo.usage = SDL::TextureUsageFlags::DEPTH_STENCIL_TARGET | SDL::TextureUsageFlags::SAMPLER;
		m_depthTexture = SDL::Texture(this, m_depthTextureInfo);

		EventHandler::On<WindowResizedEvent>([this](auto& event)
			{
				if (event.windowID == m_windowID)
				{
					m_size.x = static_cast<uint32_t>(event.x);
					m_size.y = static_cast<uint32_t>(event.y);
					m_depthTextureInfo.width = event.x;
					m_depthTextureInfo.height = event.y;
					m_depthTexture.Release();
					m_depthTexture = SDL::Texture(this, m_depthTextureInfo);
				}
				return false;
			});

		return true;
	}
	
	glm::u32vec2 Window::GetSize() const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return m_size;
	}
	uint32_t Window::GetWindowID() const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return m_windowID;
	}
	SDL::ShaderFormat Window::GetShaderFormats() const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return static_cast<SDL::ShaderFormat>(SDL_GetGPUShaderFormats(m_raw->device));
	}
	SDL::TextureFormat Window::GetSwapchainTextureFormat() const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return static_cast<SDL::TextureFormat>(SDL_GetGPUSwapchainTextureFormat(m_raw->device, m_raw->window));
	}
	void Window::WaitForGPUIdle() const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		SDL_WaitForGPUIdle(m_raw->device);
	}


	SDL::Buffer Window::CreateBuffer(const SDL::BufferInfo& info) const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return SDL::Buffer(this, info);
	}
	SDL::TransferBuffer Window::CreateTransferBuffer(const SDL::TransferBufferInfo& info) const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return SDL::TransferBuffer(this, info);
	}

	SDL::CommandBuffer Window::AcquireCommandBuffer() const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return SDL::CommandBuffer(this);
	}
	SDL::RenderPass Window::BeginRenderPass(SDL::CommandBuffer* commandBuffer, const SDL::Texture* targetTexture) const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return SDL::RenderPass(this, commandBuffer, targetTexture);
	}

	SDL::Shader Window::CreateShader(const SDL::ShaderInfo& info) const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return SDL::Shader(this, info);
	}
	SDL::GraphicsPipeline Window::CreateGraphicsPipeline(const SDL::GraphicsPipelineInfo& info) const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return SDL::GraphicsPipeline(this, info);
	}

	SDL::Texture Window::CreateTexture(const SDL::TextureInfo& info) const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return SDL::Texture(this, info);
	}
	SDL::Sampler Window::CreateSampler(const SDL::SamplerInfo& info) const
	{
		TD_CORE_ASSERT(m_raw && !m_releasedAndDestroyed);
		return SDL::Sampler(this, info);
	}
}