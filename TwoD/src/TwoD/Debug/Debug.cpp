#include "tdpch.hpp"
#include "Debug.hpp"

#ifdef TD_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

#include "TwoD/Assets/Scene.hpp"
#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Events/EventHandler.hpp"
#endif

namespace TwoD
{
	struct DebugState
	{
		bool open = false;
		bool demoWindow = false;
		EntityHandle selectedEntity;
	};
	struct PauseState
	{
		bool paused = false;
		bool nextFrame = false;
		SDL::Texture texture;
	};
#ifdef TD_IMGUI
	static DebugState s_debugState;
	static PauseState s_pauseState;
#endif

	static void SetupImGui(SDL_Window* window, SDL_GPUDevice* device)
	{
#ifdef TD_IMGUI
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
	static void SetupEventHandlers()
	{
#ifdef TD_IMGUI
		EventHandler::On<KeyDownEvent>([](const KeyDownEvent& event)
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
	static void SetupPausing(const Window& window)
	{
#ifdef TD_IMGUI
		int width, height;
		window.GetSize(width, height);
		s_pauseState.texture = window.CreateTexture({
				.type = SDL::TextureType::TWO_D,
				.format = window.GetSwapchainTextureFormat(),
				.usage = SDL::TextureUsageFlags::COLOR_TARGET,
				.width = static_cast<uint32_t>(width),
				.height = static_cast<uint32_t>(height),
				.layerCountOrDepth = 1,
				.numLevels = 1,
				.sampleCount = SDL::SampleCount::ONE,
			});
#endif
	}

	void Debug::Setup(const Window& window)
	{
#ifdef TD_IMGUI
		SetupImGui(window.m_raw->window, window.m_raw->device);
		SetupEventHandlers();
		SetupPausing(window);
#endif
	}

	void Debug::Render(const SDL::CommandBuffer& commandBuffer, const SDL::RenderPass& renderPass)
	{
#ifdef TD_IMGUI
		ImGui::Render();
		auto* drawData = ImGui::GetDrawData();
		ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer.m_raw->buffer);
		ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer.m_raw->buffer, renderPass.m_raw->renderPass, nullptr);
#endif
	}

	void Debug::HandleMultipleWindows()
	{
#ifdef TD_IMGUI
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
#endif
	}

	static void DrawPause()
	{
#ifdef TD_IMGUI
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
	static void DrawEntity(EntityHandle handle)
	{
		auto& children = handle->GetComponent<Transform>().GetChildren();
		auto* name = handle->name.c_str();
		int flags = ImGuiTreeNodeFlags_None;

		if (handle == s_debugState.selectedEntity)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (children.size() == 0)
		{
			ImGui::TreeNodeEx(name, flags | ImGuiTreeNodeFlags_Leaf);
			if (ImGui::IsItemClicked())
			{
				s_debugState.selectedEntity = handle;
			}
			ImGui::TreePop();
		}
		else if (ImGui::TreeNodeEx(name, flags))
		{
			if (ImGui::IsItemClicked())
			{
				s_debugState.selectedEntity = handle;
			}
			for (auto child : children)
			{
				DrawEntity(child);
			}
			ImGui::TreePop();
		}
	}
	static void DrawSceneInfo()
	{
#ifdef TD_IMGUI
		auto& entities = Scene::GetActive().GetRootEntities();
		for (auto entity : entities)
		{
			DrawEntity(entity);
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
#ifdef TD_IMGUI
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
#ifdef TD_IMGUI
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
	
	void Debug::Draw()
	{
#ifdef TD_IMGUI
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


#ifdef TD_IMGUI
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