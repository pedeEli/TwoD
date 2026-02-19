#pragma once
#include <string>
#include <filesystem>

#include "TwoD/Core/Base.hpp"

#include "TwoD/SDL/Window.hpp"

#include "TwoD/ECS/ECS.hpp"
#include "TwoD/Assets/AssetManager.hpp"
#include "TwoD/Assets/Scene.hpp"
#include "TwoD/Renderer/RenderSystem.hpp"
#include "TwoD/Inputs/Inputs.hpp"
#include "TwoD/Events/EventHandler.hpp"

namespace TwoD
{
	class App
	{
	public:
		struct InitInfo
		{
			int width = 0;
			int height = 0;
			std::string_view title;
			WindowFlags windowFlags = static_cast<WindowFlags>(0);
			std::string startScene;
			std::vector<std::filesystem::path> basePaths;
		};

	public:
		static App& Get();
		template<typename T>
		static constexpr T& Get()
		{
			if constexpr (std::is_same_v<T, Window>)
			{
				return Get().m_window;
			}
			else if constexpr (std::is_same_v<T, RenderSystem>)
			{
				return Get().m_renderSystem;
			}
			else
			{
				static_assert(false, "Unsuported type for App::Get<T>()");
			}
		}
		static const std::vector<std::filesystem::path>& GetBasePaths();

	public:
		App() = default;
		~App();
		App(const App& other) = delete;
		App(App&& other) = delete;
		App& operator=(const App& other) = delete;
		App& operator=(App&& other) = delete;

		void Init(const InitInfo& info);
		void Run();

		template<class T>
		requires(std::is_base_of_v<Component, T>)
		void RegisterComponent() { ECS::Register<T>(typeid(T).name()); }

		template<class T>
		requires(std::is_base_of_v<Asset, T>)
		void RegisterResource() { AssetManager::Register<T>(typeid(T).name()); }

		template<class T>
		requires(std::is_base_of_v<RenderHandler, T>)
		void RegisterRenderHandler() { m_renderSystem.RegisterHandler<T>(); }

	private:
		class SDLQuiter
		{
		public:
			~SDLQuiter();
		};

		SDLQuiter m_sdlQuiter;
		Window m_window;
		RenderSystem m_renderSystem;

		bool m_running = false;
		bool m_initialized = false;

		std::vector<std::filesystem::path> m_basePaths;
	};
}