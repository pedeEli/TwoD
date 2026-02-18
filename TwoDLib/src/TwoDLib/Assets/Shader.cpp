#include "tdpch.hpp"
#include "Shader.hpp"

#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/Core/App.hpp"

#include <SDL3/SDL_error.h>

namespace TwoD
{
	void Shader::Init(const std::filesystem::path& path)
	{
		auto& window = App::Get<Window>();

		auto vertexPath = path.parent_path() / vertex.file;
		TD_CORE_ASSERT(std::filesystem::exists(vertexPath));
		std::ifstream vertexFile(vertexPath.string());
		const auto vertexSize = std::filesystem::file_size(vertexPath);
		std::string vertexContent(vertexSize, 0);
		vertexFile.read(vertexContent.data(), vertexSize);

		auto vertexShader = SDL::Shader::Load(
			window,
			vertexContent.c_str(),
			SDL::ShaderStage::VERTEX,
			vertex.samplerCount, vertex.storageTextureCount,
			vertex.storageBufferCount, vertex.uniformBufferCount
		);
		TD_CORE_ASSERT(vertexShader, SDL_GetError());

		auto fragmentPath = path.parent_path() / fragment.file;
		TD_CORE_ASSERT(std::filesystem::exists(fragmentPath));
		std::ifstream fragmentFile(fragmentPath.string());
		const auto fragmentSize = std::filesystem::file_size(fragmentPath);
		std::string fragmentContent(fragmentSize, 0);
		fragmentFile.read(fragmentContent.data(), fragmentSize);

		auto fragmentShader = SDL::Shader::Load(
			window,
			fragmentContent.c_str(),
			SDL::ShaderStage::FRAGMENT,
			fragment.samplerCount, fragment.storageTextureCount,
			fragment.storageBufferCount, fragment.uniformBufferCount
		);
		TD_CORE_ASSERT(fragmentShader, SDL_GetError());

		for (auto& desc : pipelineInfo.targetInfo.colorTargetDescriptions)
		{
			if (desc.format == SDL::TextureFormat::INVALID)
			{
				desc.format = window.GetSwapchainTextureFormat();
			}
		}
		pipelineInfo.vertexShader = &vertexShader.value();
		pipelineInfo.fragmentShader = &fragmentShader.value();

		m_pipeline = window.CreateGraphicsPipeline(pipelineInfo);
	}

	void Shader::Bind(const SDL::RenderPass* renderPass) const
	{
		renderPass->BindGraphicsPipeline(&m_pipeline);
	}
}