#include "tdpch.hpp"
#include "App.hpp"

#include <SDL3/SDL.h>
#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Events/EventHandler.hpp"

namespace TwoD
{
	static App* s_application = nullptr;

	App& App::Get()
	{
		TD_CORE_ASSERT(s_application, "Application has to be initialized");
		return *s_application;
	}

	const std::vector<std::filesystem::path>& App::GetBasePaths()
	{
		return Get().m_basePaths;
	}

	void App::Init(const InitInfo& info)
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

		s_application = this;

		m_basePaths = info.basePaths;
		RegisterComponent<Transform>();
		RegisterResource<Scene>();
		AssetManager::Load();

		m_renderSystem.Init();

		Inputs::Init();

		AssetManager::Get<Scene>(info.startScene).SetActive();

		EventHandler::On<QuitEvent>([this](auto& e)
			{
				m_running = false;
				return false;
			});

		m_initialized = true;
	}
	App::~App()
	{
		ECS::Destroy();
		AssetManager::Unload();

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
			EventHandler::PollEvents();
			Inputs::Update();

			uint64_t currentTick = SDL_GetTicks();
			float delta = (currentTick - lastTick) / 1000.0f;
			lastTick = currentTick;
			ECS::Update(delta);
			m_renderSystem.Render();
		}
	}


	App::SDLQuiter::~SDLQuiter()
	{
		SDL_Quit();
	}
}