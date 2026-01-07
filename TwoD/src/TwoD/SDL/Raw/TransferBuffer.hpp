#pragma once
#include <SDL3/SDL.h>
#include "../TransferBuffer.hpp"

namespace TwoD::SDL
{
	struct TransferBuffer::Raw
	{
		SDL_GPUDevice* device;
		SDL_GPUTransferBuffer* buffer;
	};
}