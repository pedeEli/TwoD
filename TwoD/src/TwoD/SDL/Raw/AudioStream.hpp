#pragma once
#include <SDL3/SDL.h>
#include "../AudioStream.hpp"

namespace TwoD::SDL
{
	struct AudioStream::Raw
	{
		SDL_AudioStream* stream;
	};
}