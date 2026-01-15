#include "tdpch.hpp"
#include "App.hpp"

#include <SDL3/SDL.h>

#include "TwoD/Renderer/RenderPipeline.hpp"
#include "TwoD/Renderer/SpriteRenderLayer.hpp"
#include "TwoD/Renderer/TextRenderLayer.hpp"

namespace TwoD
{
	static App* s_application = nullptr;

	App& App::Get()
	{
		TD_CORE_ASSERT(s_application, "Application has to be initialized");
		return *s_application;
	}

	const std::filesystem::path& App::GetBasePath()
	{
		return Get().m_basePath;
	}

	
	App::App()
	{
		s_application = this;
	}

	void App::Init(const AppInfo& info)
	{
		Log::Init();
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			TD_CORE_CRITICAL("Failed to initialize sdl: {}", SDL_GetError());
			return;
		}

		WindowInfo windowInfo{
			.width = info.width,
			.height = info.height,
			.title = info.title,
			.windowFlags = info.windowFlags
		};
		if (!m_window.Init(windowInfo))
		{
			return;
		}

		m_assetManager.Load();

		RenderPipeline pipeline;
		pipeline.AddLayer<SpriteRenderLayer>();
		pipeline.AddLayer<TextRenderLayer>();
		m_renderSystem.SetPipeline(pipeline);

		m_sceneSystem.Load();
		m_sceneSystem.SetActive(info.startScene);

		s_application = this;
		m_initialized = true;
	}
	App::~App()
	{
		m_initialized = false;
		s_application = nullptr;
	}

	void App::Run()
	{
		if (!m_initialized)
		{
			return;
		}

		uint64_t lastTick = SDL_GetTicks();

		m_running = true;
		while (m_running)
		{
			m_inputSystem.Update();
			HandleEvents();

			uint64_t currentTick = SDL_GetTicks();
			float delta = (currentTick - lastTick) / 1000.0f;
			lastTick = currentTick;
			m_ecs.Update(delta);
			m_renderSystem.Render();
		}

		m_sceneSystem.Clear();
	}

	void App::HandleEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			m_inputSystem.Event(event);
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				m_running = false;
				break;
			case SDL_EVENT_WINDOW_RESIZED:
				m_window.SetSize(event.window.data1, event.window.data2);
				break;
			}
		}
	}

	void App::InitBasePath(const AppInfo& info)
	{
		if (info.basePath.empty())
		{
			m_basePath = SDL_GetBasePath();
		}
		else
		{
			m_basePath = info.basePath;
		}
	}


	App::SDLQuiter::~SDLQuiter()
	{
		SDL_Quit();
	}
}