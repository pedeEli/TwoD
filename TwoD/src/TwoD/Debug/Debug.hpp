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
		Debug() = delete;
		~Debug() = delete;
		Debug(const Debug& other) = delete;
		Debug(Debug&& other) = delete;
		Debug& operator=(const Debug& other) = delete;
		Debug& operator=(Debug&& other) = delete;

		static void Init(const Window& window);
		static void Shutdown();
		static void Render(const SDL::CommandBuffer& commandBuffer, const SDL::RenderPass& renderPass);
		static void HandleMultipleWindows();

#ifdef TD_IMGUI
		static bool GameIsRunning();
#else
		static constexpr bool GameIsRunning();
#endif

		static void Update();
	};
}