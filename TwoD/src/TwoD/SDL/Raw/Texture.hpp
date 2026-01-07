#pragma once
#include <SDL3/SDL.h>
#include "../Texture.hpp"

namespace TwoD::SDL
{
	struct Texture::Raw
	{
		SDL_GPUDevice* device;
		SDL_GPUTexture* texture;
	};
}