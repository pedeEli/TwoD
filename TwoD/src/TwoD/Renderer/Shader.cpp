#include "tdpch.hpp"
#include "Shader.hpp"

#include <SDL3_shadercross/SDL_shadercross.h>
#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void Shader::Bind(SDL::RenderPass* renderPass) const
	{
		renderPass->BindGraphicsPipeline(&pipeline);
	}
}