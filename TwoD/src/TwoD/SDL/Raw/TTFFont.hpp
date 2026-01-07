#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include "../TTFFont.hpp"

namespace TwoD::SDL
{
	struct Font::Raw
	{
		TTF_Font* font;
	};
}