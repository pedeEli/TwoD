#pragma once
#include <SDL3/SDL.h>
#include "../Sampler.hpp"

namespace TwoD::SDL
{
	struct Sampler::Raw
	{
		SDL_GPUDevice* device;
		SDL_GPUSampler* sampler;
	};
}