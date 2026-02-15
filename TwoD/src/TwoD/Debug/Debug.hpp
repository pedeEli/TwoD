#pragma once
#include "DebugDefines.hpp"
#include "TwoD/SDL/Raw/Window.hpp"
#include "TwoD/SDL/Raw/CommandBuffer.hpp"
#include "TwoD/SDL/Raw/RenderPass.hpp"

namespace TwoD
{
	class Debug
	{
	public:
		static void SetupImGui(const Window& window);
		static void StartImGuiFrame();
		static void RenderImGui(const SDL::CommandBuffer& commandBuffer, const SDL::RenderPass& renderPass);
		static void PlatformWindows();
	};
}