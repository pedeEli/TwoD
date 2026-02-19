#pragma once
#include <SDL3/SDL.h>
#include "../Fence.hpp"

namespace TwoD::SDL
{
	struct Fence::Raw
	{
		SDL_GPUFence* fence;
		SDL_GPUDevice* device;
	};
}