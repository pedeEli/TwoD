#pragma once
#include "TwoD.hpp"

TD_STRUCT(
	(TD_NAMESPACE(TwoD), TD_NAME(ShaderInfo)),
	(
		TD_STRUCT_FIELD(std::string, file),
		TD_STRUCT_FIELD(uint32_t, samplerCount, TD_INIT(0)),
		TD_STRUCT_FIELD(uint32_t, storageTextureCount, TD_INIT(0)),
		TD_STRUCT_FIELD(uint32_t, storageBufferCount, TD_INIT(0)),
		TD_STRUCT_FIELD(uint32_t, uniformBufferCount, TD_INIT(0))
	),
)

namespace TwoD
{
	class Shader : public Asset
	{
	public:
		void Init(const std::filesystem::path& path, const Window& window) override;
		void Destroy() override;

	public:
		TD_ASSET(
			TD_ASSET_FIELD(std::string, name),
			TD_ASSET_FIELD(ShaderInfo, vertex),
			TD_ASSET_FIELD(ShaderInfo, fragment),
			TD_ASSET_FIELD(SDL::GraphicsPipelineInfo, pipelineInfo, {})
		)

	public:
		void Bind(const SDL::RenderPass* renderPass) const;

	private:
		SDL::GraphicsPipeline m_pipeline;
	};
}