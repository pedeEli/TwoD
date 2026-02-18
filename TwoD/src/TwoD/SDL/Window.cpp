#include "tdpch.hpp"
#include "Window.hpp"

#include "Raw/Window.hpp"
#include "TwoD/Events/EventHandler.hpp"

namespace TwoD
{
	Window::Window() = default;
	Window::~Window()
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

		if (!SDL_GetWindowSize(window, &m_width, &m_height))
		{
			TD_CORE_CRITICAL("Failed to get window size: {}", SDL_GetError());
		}

		m_windowID = SDL_GetWindowID(window);

		m_raw = std::make_unique<Raw>(window, device);

		m_depthTextureInfo.type = SDL::TextureType::TWO_D;
		m_depthTextureInfo.width = m_width;
		m_depthTextureInfo.height = m_height;
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
					m_width = event.x;
					m_height = event.y;
					m_depthTextureInfo.width = event.x;
					m_depthTextureInfo.height = event.y;
					m_depthTexture = SDL::Texture(this, m_depthTextureInfo);
				}
				return false;
			});

		return true;
	}
	
	void Window::GetSize(int& width, int& height) const
	{
		width = m_width;
		height = m_height;
	}
	uint32_t Window::GetWindowID() const
	{
		return m_windowID;
	}
	SDL::ShaderFormat Window::GetShaderFormats() const
	{
		return static_cast<SDL::ShaderFormat>(SDL_GetGPUShaderFormats(m_raw->device));
	}
	SDL::TextureFormat Window::GetSwapchainTextureFormat() const
	{
		return static_cast<SDL::TextureFormat>(SDL_GetGPUSwapchainTextureFormat(m_raw->device, m_raw->window));
	}
}