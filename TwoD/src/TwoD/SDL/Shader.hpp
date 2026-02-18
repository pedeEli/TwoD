#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
	class Shader
	{
	public:
		static std::optional<SDL::Shader> Load(
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
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

	private:
		struct Raw;
		std::unique_ptr<Raw> m_raw;

		friend class GraphicsPipeline;
	};
}


