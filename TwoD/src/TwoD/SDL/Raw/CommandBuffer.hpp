#pragma once
#include <SDL3/SDL.h>
#include "../CommandBuffer.hpp"

namespace TwoD::SDL
{
	struct CommandBuffer::Raw
	{
		SDL_GPUCommandBuffer* buffer;
	};
}