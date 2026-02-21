#include "tdpch.hpp"
#include "GraphicsPipeline.hpp"

#include "Window.hpp"
#include "Shader.hpp"
#include "Raw/Window.hpp"
#include "Raw/Shader.hpp"
#include "Raw/GraphicsPipeline.hpp"

namespace TwoD::SDL
{
	GraphicsPipeline::GraphicsPipeline() = default;
	GraphicsPipeline::GraphicsPipeline(const Window* window, const GraphicsPipelineInfo& info)
	{
		TD_CORE_ASSERT(info.vertexShader->m_raw && !info.vertexShader->m_released);
		TD_CORE_ASSERT(info.fragmentShader->m_raw && !info.fragmentShader->m_released);
		auto* device = window->m_raw->device;
		
		std::vector<SDL_GPUVertexBufferDescription> vertexBufferDescriptions(
			info.vertexInputState.vertexBufferDescriptions.size()
		);
		std::vector<SDL_GPUVertexAttribute> vertexAttributes(
			info.vertexInputState.vertexAttributes.size()
		);
		std::vector<SDL_GPUColorTargetDescription> colorTargetDescriptions(
			info.targetInfo.colorTargetDescriptions.size()
		);

		for (size_t i = 0; i < vertexBufferDescriptions.size(); i++)
		{
			const auto& desc = info.vertexInputState.vertexBufferDescriptions[i];
			vertexBufferDescriptions[i].slot = desc.slot;
			vertexBufferDescriptions[i].pitch = desc.pitch;
			vertexBufferDescriptions[i].input_rate = static_cast<SDL_GPUVertexInputRate>(desc.inputRate);
		}
		for (size_t i = 0; i < vertexAttributes.size(); i++)
		{
			const auto& attr = info.vertexInputState.vertexAttributes[i];
			vertexAttributes[i].location = attr.location;
			vertexAttributes[i].buffer_slot = attr.bufferSlot;
			vertexAttributes[i].format = static_cast<SDL_GPUVertexElementFormat>(attr.format);
			vertexAttributes[i].offset = attr.offset;
		}
		for (size_t i = 0; i < colorTargetDescriptions.size(); i++)
		{
			const auto& desc = info.targetInfo.colorTargetDescriptions[i];
			colorTargetDescriptions[i].format = static_cast<SDL_GPUTextureFormat>(desc.format);
			colorTargetDescriptions[i].blend_state.src_color_blendfactor = static_cast<SDL_GPUBlendFactor>(desc.blendState.srcColorBlendfactor);
			colorTargetDescriptions[i].blend_state.dst_color_blendfactor = static_cast<SDL_GPUBlendFactor>(desc.blendState.dstColorBlendfactor);
			colorTargetDescriptions[i].blend_state.color_blend_op = static_cast<SDL_GPUBlendOp>(desc.blendState.colorBlendOp);
			colorTargetDescriptions[i].blend_state.src_alpha_blendfactor = static_cast<SDL_GPUBlendFactor>(desc.blendState.srcAlphaBlendfactor);
			colorTargetDescriptions[i].blend_state.dst_alpha_blendfactor = static_cast<SDL_GPUBlendFactor>(desc.blendState.dstAlphaBlendfactor);
			colorTargetDescriptions[i].blend_state.alpha_blend_op = static_cast<SDL_GPUBlendOp>(desc.blendState.alphaBlendOp);
			colorTargetDescriptions[i].blend_state.color_write_mask = static_cast<SDL_GPUColorComponentFlags>(desc.blendState.colorWriteMask);
			colorTargetDescriptions[i].blend_state.enable_blend = desc.blendState.enableBlend;
			colorTargetDescriptions[i].blend_state.enable_color_write_mask = desc.blendState.enableColorWriteMask;
		}

		SDL_GPUGraphicsPipelineCreateInfo sdlInfo{
			.vertex_shader = info.vertexShader->m_raw->shader,
			.fragment_shader = info.fragmentShader->m_raw->shader,
			.vertex_input_state = {
				.vertex_buffer_descriptions = vertexBufferDescriptions.data(),
				.num_vertex_buffers = static_cast<uint32_t>(vertexBufferDescriptions.size()),
				.vertex_attributes = vertexAttributes.data(),
				.num_vertex_attributes = static_cast<uint32_t>(vertexAttributes.size())
			},
			.primitive_type = static_cast<SDL_GPUPrimitiveType>(info.primitiveType),
			.rasterizer_state = {
				.fill_mode = static_cast<SDL_GPUFillMode>(info.rasterizerState.fillMode),
				.cull_mode = static_cast<SDL_GPUCullMode>(info.rasterizerState.cullMode),
				.front_face = static_cast<SDL_GPUFrontFace>(info.rasterizerState.frontFace),
				.depth_bias_constant_factor = info.rasterizerState.depthBiasConstantFactor,
				.depth_bias_clamp = info.rasterizerState.depthBiasClamp,
				.depth_bias_slope_factor = info.rasterizerState.depthBiasSlopeFactor,
				.enable_depth_bias = info.rasterizerState.enableDepthBias,
				.enable_depth_clip = info.rasterizerState.enableDepthClip
			},
			.multisample_state = {
				.sample_count = static_cast<SDL_GPUSampleCount>(info.multisampleState.sampleCount),
				.sample_mask = info.multisampleState.sampleMask,
				.enable_mask = info.multisampleState.enableMask
			},
			.depth_stencil_state = {
				.compare_op = static_cast<SDL_GPUCompareOp>(info.depthStencilState.compareOp),
				.back_stencil_state = {
					.fail_op = static_cast<SDL_GPUStencilOp>(info.depthStencilState.backStencilState.failOp),
					.pass_op = static_cast<SDL_GPUStencilOp>(info.depthStencilState.backStencilState.passOp),
					.depth_fail_op = static_cast<SDL_GPUStencilOp>(info.depthStencilState.backStencilState.depthFailOp),
					.compare_op = static_cast<SDL_GPUCompareOp>(info.depthStencilState.backStencilState.compareOp)
				},
				.front_stencil_state = {
					.fail_op = static_cast<SDL_GPUStencilOp>(info.depthStencilState.frontStencilState.failOp),
					.pass_op = static_cast<SDL_GPUStencilOp>(info.depthStencilState.frontStencilState.passOp),
					.depth_fail_op = static_cast<SDL_GPUStencilOp>(info.depthStencilState.frontStencilState.depthFailOp),
					.compare_op = static_cast<SDL_GPUCompareOp>(info.depthStencilState.frontStencilState.compareOp)
				},
				.compare_mask = info.depthStencilState.compareMask,
				.write_mask = info.depthStencilState.writeMask,
				.enable_depth_test = info.depthStencilState.enableDepthTest,
				.enable_depth_write = info.depthStencilState.enableDepthWrite,
				.enable_stencil_test = info.depthStencilState.enableStencilTest
			},
			.target_info = {
				.color_target_descriptions = colorTargetDescriptions.data(),
				.num_color_targets = static_cast<uint32_t>(colorTargetDescriptions.size()),
				.depth_stencil_format = static_cast<SDL_GPUTextureFormat>(info.targetInfo.depthStencilFormat),
				.has_depth_stencil_target = info.targetInfo.hasDepthStencilTarget
			}
		};

		auto* pipeline = SDL_CreateGPUGraphicsPipeline(device, &sdlInfo);
		m_raw = std::make_unique<Raw>(device, pipeline);
	}
	GraphicsPipeline::~GraphicsPipeline()
	{
		TD_CORE_ASSERT(!m_raw || m_released);
	}

	void GraphicsPipeline::Release()
	{
		TD_CORE_ASSERT(!m_released);
		m_released = true;
		if (m_raw)
		{
			SDL_ReleaseGPUGraphicsPipeline(m_raw->device, m_raw->pipeline);
		}
	}

	void GraphicsPipeline::swap(GraphicsPipeline&& other)
	{
		std::swap(m_raw, other.m_raw);
		std::swap(m_released, other.m_released);
	}

	GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) noexcept
	{
		swap(std::move(other));
	}

	GraphicsPipeline& GraphicsPipeline::operator=(GraphicsPipeline&& other) noexcept
	{
		if (this != &other)
		{
			swap(std::move(other));
		}
		return *this;
	}
}