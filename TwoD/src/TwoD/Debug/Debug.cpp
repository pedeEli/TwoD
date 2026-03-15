#include "tdpch.hpp"
#include "Debug.hpp"

#ifdef TD_CREATE_DEBUGGER
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

#include "TwoD/Assets/Scene.hpp"
#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Events/EventHandler.hpp"

#include "TwoD/SDL/Raw/Window.hpp"
#include "TwoD/SDL/Raw/CommandBuffer.hpp"
#include "TwoD/SDL/Raw/RenderPass.hpp"
#endif

namespace TwoD
{
	struct DebugState
	{
		bool open = false;
		bool demoWindow = false;
		EntityHandle selectedEntity;
		EventHandler::Handle keyDownHandle;
	};
	struct PauseState
	{
		bool paused = false;
		bool nextFrame = false;
	};
#ifdef TD_CREATE_DEBUGGER
	static DebugState s_debugState;
	static PauseState s_pauseState;
#endif

	static void InitImGui(SDL_Window* window, SDL_GPUDevice* device)
	{
#ifdef TD_CREATE_DEBUGGER
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigViewportsNoAutoMerge = false;
		io.ConfigViewportsNoTaskBarIcon = false;

		ImGui::StyleColorsDark();

		auto& style = ImGui::GetStyle();
		auto mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
		style.ScaleAllSizes(mainScale);
		style.FontScaleDpi = mainScale;

		ImGui_ImplSDL3_InitForSDLGPU(window);
		ImGui_ImplSDLGPU3_InitInfo initInfo{
			.Device = device,
			.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window),
			.MSAASamples = SDL_GPU_SAMPLECOUNT_1,
			.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
			.PresentMode = SDL_GPU_PRESENTMODE_VSYNC
		};
		ImGui_ImplSDLGPU3_Init(&initInfo);
#endif
	}
	static void InitEventHandlers()
	{
#ifdef TD_CREATE_DEBUGGER
		s_debugState.keyDownHandle = EventHandler::Add<KeyDownEvent>([](const KeyDownEvent& event)
			{
				if (event.repeat || (event.mod & Keymod::CTRL) == Keymod::NONE)
				{
					return false;
				}
				if (event.key == Key::F5)
				{
					s_debugState.open = !s_debugState.open;
				}
				return false;
			});
#endif
	}

	void Debug::Init(const Window& window)
	{
#ifdef TD_CREATE_DEBUGGER
		TD_CORE_ASSERT(window.m_raw && !window.m_releasedAndDestroyed);
		InitImGui(window.m_raw->window, window.m_raw->device);
		InitEventHandlers();
#endif
	}
	void Debug::Shutdown()
	{
#ifdef TD_CREATE_DEBUGGER
		ImGui_ImplSDL3_Shutdown();
		ImGui_ImplSDLGPU3_Shutdown();
		ImGui::DestroyContext();
		EventHandler::Remove(s_debugState.keyDownHandle);
#endif
	}

	void Debug::Render(const SDL::CommandBuffer& commandBuffer, const SDL::RenderPass& renderPass)
	{
#ifdef TD_CREATE_DEBUGGER
		TD_CORE_ASSERT(commandBuffer.m_raw && !commandBuffer.m_submitted);
		TD_CORE_ASSERT(renderPass.m_raw && !renderPass.m_ended);
		ImGui::Render();
		auto* drawData = ImGui::GetDrawData();
		ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer.m_raw->buffer);
		ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer.m_raw->buffer, renderPass.m_raw->renderPass, nullptr);
#endif
	}

	void Debug::HandleMultipleWindows()
	{
#ifdef TD_CREATE_DEBUGGER
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
#endif
	}

	static void DrawPause()
	{
#ifdef TD_CREATE_DEBUGGER
		if (s_pauseState.paused)
		{
			if (ImGui::Button("resume"))
			{
				s_pauseState.paused = false;
			}
		}
		else
		{
			if (ImGui::Button("pause"))
			{
				s_pauseState.paused = true;
			}
		}

		ImGui::SameLine();
		s_pauseState.nextFrame = false;
		if (ImGui::Button("next frame"))
		{
			s_pauseState.nextFrame = true;
		}
#endif
	}
	static void DrawEntity(EntityHandle handle, uint32_t& id)
	{
		auto& children = handle->GetTransform()->GetChildren();
		auto name = std::format("{}##{}", handle->name, id++);
		int flags = ImGuiTreeNodeFlags_None;

		if (handle == s_debugState.selectedEntity)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (children.size() == 0)
		{
			ImGui::TreeNodeEx(name.c_str(), flags | ImGuiTreeNodeFlags_Leaf);
			if (ImGui::IsItemClicked())
			{
				s_debugState.selectedEntity = handle;
			}
			ImGui::TreePop();
		}
		else if (ImGui::TreeNodeEx(name.c_str(), flags))
		{
			if (ImGui::IsItemClicked())
			{
				s_debugState.selectedEntity = handle;
			}
			for (auto child : children)
			{
				DrawEntity(child, id);
			}
			ImGui::TreePop();
		}
	}
	static void DrawSceneInfo()
	{
#ifdef TD_CREATE_DEBUGGER
		auto& scene = Scene::GetActive();
		auto& entities = scene.GetRootEntities();
		uint32_t id = 0;
		for (auto entity : entities)
		{
			DrawEntity(entity, id);
		}

		auto screen = scene.GetScreenRootEntity();
		if (screen)
		{
			DrawEntity(screen, id);
		}
#endif
	}
	static void DrawComponents(EntityHandle handle)
	{
		ImGui::Text(handle->name.c_str());
		auto components = handle->GetAllComponents();
		for (auto component : components)
		{
			if (ImGui::CollapsingHeader(component.first.name()))
			{
				component.second->Debug();
			}
		}
	}
	static void DrawEntityInfo()
	{
#ifdef TD_CREATE_DEBUGGER
		bool components = s_debugState.selectedEntity;
		if (components)
		{
			if (ImGui::Begin("Components", &components))
			{
				DrawComponents(s_debugState.selectedEntity);
			}
			ImGui::End();
			if (!components)
			{
				s_debugState.selectedEntity = EntityHandle::None;
			}
		}
#endif
	}
	static void DrawDebug()
	{
#ifdef TD_CREATE_DEBUGGER
		if (!s_debugState.open)
		{
			return;
		}
		if (ImGui::Begin("Debug", &s_debugState.open))
		{
			if (ImGui::Button("Open demo window"))
			{
				s_debugState.demoWindow = true;
			}
			DrawPause();
			DrawSceneInfo();
		}
		ImGui::End();
		DrawEntityInfo();
#endif
	}
	
	void Debug::Update()
	{
#ifdef TD_CREATE_DEBUGGER
		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		
		if (s_debugState.demoWindow)
		{
			ImGui::ShowDemoWindow(&s_debugState.demoWindow);
		}

		DrawDebug();
#endif
	}


#ifdef TD_CREATE_DEBUGGER
	bool Debug::GameIsRunning()
	{
		return !s_pauseState.paused || s_pauseState.nextFrame;
	}
#else
	constexpr bool Debug::IsGameRunning()
	{
		return true;
	}
#endif
}