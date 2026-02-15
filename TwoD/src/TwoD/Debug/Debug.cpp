#include "tdpch.hpp"
#include "Debug.hpp"

#ifdef TD_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>
#endif

namespace TwoD
{
	void Debug::SetupImGui(const Window& window)
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

		ImGui_ImplSDL3_InitForSDLGPU(window.m_raw->window);
		ImGui_ImplSDLGPU3_InitInfo initInfo{
			.Device = window.m_raw->device,
			.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(window.m_raw->device, window.m_raw->window),
			.MSAASamples = SDL_GPU_SAMPLECOUNT_1,
			.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
			.PresentMode = SDL_GPU_PRESENTMODE_VSYNC
		};
		ImGui_ImplSDLGPU3_Init(&initInfo);
#endif
	}

	void Debug::StartImGuiFrame()
	{
#ifdef TD_IMGUI
		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
#endif
	}

	void Debug::RenderImGui(const SDL::CommandBuffer& commandBuffer, const SDL::RenderPass& renderPass)
	{
#ifdef TD_IMGUI
		ImGui::Render();
		auto* drawData = ImGui::GetDrawData();
		ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer.m_raw->buffer);
		ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer.m_raw->buffer, renderPass.m_raw->renderPass, nullptr);
#endif
	}

	void Debug::PlatformWindows()
	{
#ifdef TD_IMGUI
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
#endif
	}
}