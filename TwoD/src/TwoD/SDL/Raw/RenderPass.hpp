#pragma once
#include <SDL3/SDL.h>
#include "../RenderPass.hpp"

namespace TwoD::SDL
{
	struct RenderPass::Raw
	{
		SDL_GPURenderPass* renderPass;
	};
}