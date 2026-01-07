#pragma once
#include <SDL3/SDL.h>
#include "../Window.hpp"

namespace TwoD
{
	struct Window::Raw
	{
		SDL_Window* window;
		SDL_GPUDevice* device;
	};
}