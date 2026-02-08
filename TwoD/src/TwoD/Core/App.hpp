#pragma once
#include <string>
#include <filesystem>

#include "TwoD/Core/Base.hpp"

#include "TwoD/SDL/Window.hpp"
#include "TwoD/ECS/ECS.hpp"

#include "TwoD/Assets/AssetManager.hpp"

#include "TwoD/Renderer/RenderSystem.hpp"

#include "TwoD/Systems/SceneSystem.hpp"
#include "TwoD/Systems/InputSystem.hpp"

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
			else if constexpr (std::is_same_v<T, ECS>)
			{
				return Get().m_ecs;
			}
			else if constexpr (std::is_same_v<T, AssetManager>)
			{
				return Get().m_assetManager;
			}
			else if constexpr (std::is_same_v<T, SceneSystem>)
			{
				return Get().m_sceneSystem;
			}
			else if constexpr (std::is_same_v<T, RenderSystem>)
			{
				return Get().m_renderSystem;
			}
			else if constexpr (std::is_same_v<T, InputSystem>)
			{
				return Get().m_inputSystem;
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
		void Init(const InitInfo& info);
		void Run();

		template<class T>
		requires(std::is_base_of_v<Component, T>)
		void RegisterComponent() { m_ecs.Register<T>(); }

		template<class T>
		requires(std::is_base_of_v<Asset, T>)
		void RegisterResource() { m_assetManager.Register<T>(); }

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
		SceneSystem m_sceneSystem;
		RenderSystem m_renderSystem;
		ECS m_ecs;
		AssetManager m_assetManager;
		InputSystem m_inputSystem;

		bool m_running = false;
		bool m_initialized = false;

		std::vector<std::filesystem::path> m_basePaths;
	};
}

#include "TwoD/Assets/AssetManager.inl"