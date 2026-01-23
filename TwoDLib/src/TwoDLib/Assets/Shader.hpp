#pragma once

#include "TwoD/Core/YAML.hpp"
#include "TwoD/Assets/AssetManager.hpp"
#include "TwoD/SDL/GraphicsPipeline.hpp"


TD_YAML_STRUCT(TwoD, ShaderInfo,
	TD_YAML_STRUCT_FIELD(std::string, file),
	TD_YAML_STRUCT_FIELD(uint32_t, samplerCount),
	TD_YAML_STRUCT_FIELD(uint32_t, storageTextureCount),
	TD_YAML_STRUCT_FIELD(uint32_t, storageBufferCount),
	TD_YAML_STRUCT_FIELD(uint32_t, uniformBufferCount)
)

namespace TwoD
{
	class Shader : public Asset
	{
	public:
		void Init(const std::filesystem::path& path) override;

	public:
		TD_ASSET_FIELDS(
			TD_FIELD(std::string, file),
			TD_FIELD(std::string, name),
			TD_FIELD(ShaderInfo, vertex),
			TD_FIELD(ShaderInfo, fragment),
			TD_FIELD(SDL::GraphicsPipelineInfo, pipelineInfo)
		)

	public:
		void Bind(SDL::RenderPass* renderPass) const;

	private:
		SDL::GraphicsPipeline m_pipeline;
	};
}