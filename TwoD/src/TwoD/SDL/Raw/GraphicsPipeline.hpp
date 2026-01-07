#pragma once
#include <SDL3/SDL.h>
#include "../GraphicsPipeline.hpp"

namespace TwoD::SDL
{
	struct GraphicsPipeline::Raw
	{
		SDL_GPUDevice* device;
		SDL_GPUGraphicsPipeline* pipeline;
	};
}