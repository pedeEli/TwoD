#pragma once
#include "SDLDefines.hpp"

namespace TwoD::SDL
{
	class Shader
	{
	public:
		static SDL::Shader Load(
			const Window& window,
			const char* hlsl,
			SDL::ShaderStage stage,
			uint32_t samplerCount,
			uint32_t storageTextureCount,
			uint32_t storageBufferCount,
			uint32_t uniformBufferCount
		);

	public:
		Shader();
		Shader(const Window* window, const ShaderInfo& info);
		~Shader();
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept;

		void Release();
		void swap(Shader&& other);

	private:
		bool m_released = false;
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class GraphicsPipeline;
	};
}


