#pragma once
#include <SDL3/SDL.h>
#include "../Surface.hpp"

namespace TwoD::SDL
{
	struct Surface::Raw
	{
		SDL_Surface* surface;
	};
}