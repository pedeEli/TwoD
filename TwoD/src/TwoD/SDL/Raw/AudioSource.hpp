#pragma once
#include <SDL3/SDL.h>
#include "../AudioSource.hpp"

namespace TwoD::SDL
{
	struct AudioSource::Raw
	{
		SDL_AudioSpec specs;
		uint8_t* buffer;
		uint32_t length;
	};
}