#include "tdpch.hpp"
#include "Shader.hpp"

#include <SDL3_shadercross/SDL_shadercross.h>
#include "TwoD/Core/App.hpp"
#include "Window.hpp"
#include "Raw/Window.hpp"
#include "Raw/Shader.hpp"

namespace TwoD::SDL
{
	std::optional<SDL::Shader> Shader::Load(
		const Window& window,
		const char* hlsl,
		SDL::ShaderStage stage,
		uint32_t samplerCount,
		uint32_t storageTextureCount,
		uint32_t storageBufferCount,
		uint32_t uniformBufferCount
	)
	{
		SDL::ShaderFormat backendFormats = window.GetShaderFormats();
		SDL::ShaderFormat format = SDL::ShaderFormat::INVALID;
		const char* entrypoint;

		if ((backendFormats & SDL::ShaderFormat::SPIRV) != SDL::ShaderFormat::INVALID)
		{
			entrypoint = "main";
			format = SDL::ShaderFormat::SPIRV;
		}
		else if ((backendFormats & SDL::ShaderFormat::MSL) != SDL::ShaderFormat::INVALID)
		{
			entrypoint = "main0";
			format = SDL::ShaderFormat::MSL;
		}
		else if ((backendFormats & SDL::ShaderFormat::DXIL) != SDL::ShaderFormat::INVALID)
		{
			entrypoint = "main";
			format = SDL::ShaderFormat::DXIL;
		}
		else
		{
			TD_CORE_ERROR("Not supported shader format");
			return {};
		}

		SDL_ShaderCross_ShaderStage shaderCrossStage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;
		if (stage == SDL::ShaderStage::FRAGMENT)
		{
			shaderCrossStage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;
		}

		size_t codeSize;
		uint8_t* code;
		SDL_ShaderCross_HLSL_Info info{
			.source = hlsl,
			.entrypoint = "main",
			.shader_stage = shaderCrossStage,
			.props = 0
		};
		if ((format & (SDL::ShaderFormat::SPIRV | SDL::ShaderFormat::MSL)) != SDL::ShaderFormat::INVALID)
		{
			code = (uint8_t*)SDL_ShaderCross_CompileSPIRVFromHLSL(&info, &codeSize);
		}
		else
		{
			code = (uint8_t*)SDL_ShaderCross_CompileDXILFromHLSL(&info, &codeSize);
		}
		if ((format & SDL::ShaderFormat::MSL) != SDL::ShaderFormat::INVALID)
		{
			SDL_ShaderCross_SPIRV_Info spirvInfo{
				.bytecode = code,
				.bytecode_size = codeSize,
				.entrypoint = "main",
				.shader_stage = shaderCrossStage,
				.props = 0
			};
			char* tempCode = (char*)SDL_ShaderCross_TranspileMSLFromSPIRV(&spirvInfo);
			SDL_free(code);
			codeSize = std::strlen(tempCode);
			code = (uint8_t*)tempCode;
		}

		SDL::ShaderInfo shaderCreateInfo{
			.codeSize = static_cast<uint32_t>(codeSize),
			.code = code,
			.entrypoint = entrypoint,
			.format = format,
			.stage = stage,
			.numSamplers = samplerCount,
			.numStorageTextures = storageTextureCount,
			.numStorageBuffers = storageBufferCount,
			.numUniformBuffers = uniformBufferCount
		};
		auto shader = window.CreateShader(shaderCreateInfo);
		SDL_free(code);
		return shader;
	}

	Shader::Shader() = default;
	Shader::Shader(const Window* window, const ShaderInfo& info)
	{
		auto* device = window->m_raw->device;
		SDL_GPUShaderCreateInfo sdlInfo{
			.code_size = info.codeSize,
			.code = info.code,
			.entrypoint = info.entrypoint.c_str(),
			.format = static_cast<SDL_GPUShaderFormat>(info.format),
			.stage = static_cast<SDL_GPUShaderStage>(info.stage),
			.num_samplers = info.numSamplers,
			.num_storage_textures = info.numStorageTextures,
			.num_storage_buffers = info.numStorageBuffers,
			.num_uniform_buffers = info.numUniformBuffers
		};
		auto* shader = SDL_CreateGPUShader(device, &sdlInfo);
		TD_CORE_ASSERT(shader != nullptr, SDL_GetError());
		m_raw = std::make_unique<Raw>(device, shader);
	}
	Shader::~Shader()
	{
		if (m_raw)
		{
			SDL_ReleaseGPUShader(m_raw->device, m_raw->shader);
		}
	}

	Shader::Shader(Shader&& other) noexcept = default;
	Shader& Shader::operator=(Shader&& other) noexcept = default;
}
