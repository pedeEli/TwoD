#pragma once
#include <SDL3/SDL.h>
#include "../Shader.hpp"

namespace TwoD::SDL
{
	struct Shader::Raw
	{
		SDL_GPUDevice* device;
		SDL_GPUShader* shader;
	};
}