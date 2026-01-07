#pragma once
#include <SDL3/SDL.h>
#include "../Buffer.hpp"

namespace TwoD::SDL
{
	struct Buffer::Raw
	{
		SDL_GPUDevice* device;
		SDL_GPUBuffer* buffer;
	};
}