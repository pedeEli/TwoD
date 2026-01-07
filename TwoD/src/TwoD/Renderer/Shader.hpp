#pragma once
#include "TwoD/SDL/Defines.hpp"
#include "TwoD/SDL/GraphicsPipeline.hpp"

namespace TwoD
{
	class Shader
	{
	public:
		Shader() {}
		~Shader() = default;
		Shader(Shader&& other) noexcept = default;
		Shader& operator=(Shader&& other) noexcept = default;
		
		void Bind(SDL::RenderPass* renderPass) const;

	public:
		SDL::GraphicsPipeline pipeline;
	};
}