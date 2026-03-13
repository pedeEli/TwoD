#include "tdpch.hpp"
#include "App.hpp"

#include <SDL3/SDL.h>
#include "TwoD/ECS/Transform.hpp"
#include "TwoD/ECS/UITransform.hpp"
#include "TwoD/Events/EventHandler.hpp"

#include "TwoD/Debug/Debug.hpp"

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
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
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
		RegisterComponent<UITransform>();
		RegisterResource<Scene>();
		
		AudioDevices::Init();
		Inputs::Init(m_window);
		AssetManager::Init(m_window);
		m_renderSystem.Init(m_window);
		Debug::Init(m_window);

		AssetManager::Get<Scene>(info.startScene).SetActive();

		EventHandler::On<WindowCloseRequestedEvent>([this](auto& e)
			{
				if (e.windowID == m_window.GetWindowID())
				{
					m_running = false;
				}
				return false;
			});

		m_initialized = true;

	}
	App::~App()
	{
		AssetManager::Shutdown();
		m_renderSystem.Shutdown();
		AudioDevices::Shutdown();
		ECS::Shutdown();
		Debug::Shutdown();
		m_window.WaitForGPUIdle();
		m_window.ReleaseAndDestroy();
		SDL_Quit();

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
			EventHandler::Update();
			Inputs::Update();

			Debug::Update();

			uint64_t currentTick = SDL_GetTicks();
			float delta = (currentTick - lastTick) / 1000.0f;
			lastTick = currentTick;
			if (Debug::GameIsRunning())
			{
				ECS::Update(delta);
				Scene::GetActive().UpdateMatrices();
			}

			m_renderSystem.Update(m_window);

			Debug::HandleMultipleWindows();
		}
	}
}