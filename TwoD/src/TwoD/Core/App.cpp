#include "tdpch.hpp"
#include "App.hpp"

#include <SDL3/SDL.h>
#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Events/EventHandler.hpp"

#include "TwoD/Debug/Debug.hpp"
#include <imgui.h>


namespace TwoD
{
	static bool s_check = false;
	static float s_float = 0.0f;

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

		EventHandler::On<WindowCloseRequestedEvent>([this](auto& e)
			{
				if (e.windowID == m_window.GetWindowID())
				{
					m_running = false;
				}
				return false;
			});

		m_initialized = true;

		TwoD::Debug::SetupImGui(m_window);
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

			Debug::StartImGuiFrame();

			uint64_t currentTick = SDL_GetTicks();
			float delta = (currentTick - lastTick) / 1000.0f;
			lastTick = currentTick;
			ECS::Update(delta);

			ImGui::Begin("hello world", nullptr);
			ImGui::Text("This is some text");
			ImGui::Checkbox("checkbox:", &s_check);
			ImGui::SliderFloat("floats: ", &s_float, 0.0f, 10.0f);
			ImGui::End();

			m_renderSystem.Render();

			Debug::PlatformWindows();
		}
	}


	App::SDLQuiter::~SDLQuiter()
	{
		SDL_Quit();
	}
}