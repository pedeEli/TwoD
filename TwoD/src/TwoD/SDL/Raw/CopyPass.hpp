#pragma once
#include <SDL3/SDL.h>
#include "../CopyPass.hpp"

namespace TwoD::SDL
{
	struct CopyPass::Raw
	{
		SDL_GPUCopyPass* copyPass;
	};
}