#include "tdpch.hpp"
#include "TextRenderLayer.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoD/Components/Camera.hpp"

extern const char* vertex;
extern const char* fragment;

namespace TwoD
{
	std::optional<Shader> TextRenderLayer::LoadShader()
	{
		auto& window = App::Get<Window>();

		auto vertexShader = SDL::Shader::Load(
			window,
			vertex,
			SDL::ShaderStage::VERTEX,
			0, 0, 1, 1
		);
		if (!vertexShader)
		{
			TD_CORE_ERROR("Failed to load text vertex shader");
			return {};
		}

		auto fragmentShader = SDL::Shader::Load(
			window,
			fragment,
			SDL::ShaderStage::FRAGMENT,
			1, 0, 0, 0
		);
		if (!fragmentShader)
		{
			TD_CORE_ERROR("Failed to load text fragment shader");
			return {};
		}

		SDL::ColorTargetDescription colorTargetDescription{
			.format = window.GetSwapchainTextureFormat(),
			.blendState = {
				.srcColorBlendfactor = SDL::BlendFactor::SRC_ALPHA,
				.dstColorBlendfactor = SDL::BlendFactor::ONE_MINUS_SRC_ALPHA,
				.colorBlendOp = SDL::BlendOp::ADD,
				.srcAlphaBlendfactor = SDL::BlendFactor::ONE,
				.dstAlphaBlendfactor = SDL::BlendFactor::ZERO,
				.alphaBlendOp = SDL::BlendOp::ADD,
				.colorWriteMask = SDL::ColorComponentFlags::R | SDL::ColorComponentFlags::G | SDL::ColorComponentFlags::B | SDL::ColorComponentFlags::A,
				.enableBlend = true,
				.enableColorWriteMask = true
			}
		};

		SDL::VertexBufferDescription vertexBufferDescription{
			.slot = 0,
			.pitch = 0,
			.inputRate = SDL::VertexInputRate::VERTEX
		};

		SDL::GraphicsPipelineInfo pipelineInfo{
			.vertexShader = &vertexShader.value(),
			.fragmentShader = &fragmentShader.value(),
			.vertexInputState = {
				.vertexBufferDescriptions = {vertexBufferDescription},
				.vertexAttributes = {},
			},
			.primitiveType = SDL::PrimitiveType::TRIANGLELIST,
			.rasterizerState = {
				.fillMode = SDL::FillMode::FILL
			},
			.depthStencilState = {
				.compareOp = SDL::CompareOp::LESS,
				.enableDepthTest = true,
				.enableDepthWrite = false
			},
			.targetInfo = {
				.colorTargetDescriptions = {colorTargetDescription},
				.depthStencilFormat = SDL::TextureFormat::D24_UNORM,
				.hasDepthStencilTarget = true
			}
		};

		auto pipeline = window.CreateGraphicsPipeline(pipelineInfo);
		Shader shader;
		shader.pipeline = std::move(pipeline);
		return shader;
	}

	TextRenderLayer::TextRenderLayer()
	{
		auto shader = LoadShader();
		if (shader)
		{
			m_shader = std::move(*shader);
		}
		App::Get<AssetManager>().Get<SpriteAtlas>("font-atlas").Pack();
	}

	void TextRenderLayer::Render(ECS& ecs, SDL::CommandBuffer& commandBuffer, SDL::RenderPass& renderPass) const
	{
		auto& renderers = ecs.GetComponents<TextRenderer>();

		m_shader.Bind(&renderPass);
		auto& atlas = App::Get<AssetManager>().Get<SpriteAtlas>("font-atlas");
		atlas.Bind(&renderPass);

		auto camera = Camera::Get();
		Uniform uniform{
			camera->GetProjectionMatrix(),
			camera->GetWorldToCameraMatrix()
		};

		size_t i = 0;
		auto data = m_transferBuffer.Map<Instance>(true);
		for (auto index : m_indices)
		{
			auto& renderer = renderers[index];
			uniform.model = renderer.GetComponent<Transform>()->GetWorldMatrix();
			const auto& rects = renderer.GetRects();

			for (size_t i = 0; i < rects.size(); i++)
			{
				auto& info = rects[i];
				data[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
				data[i].tex = info.first;
				data[i].glyph = info.second;
			}

			auto size = static_cast<uint32_t>(sizeof(Instance) * rects.size());
			auto copyPass = commandBuffer.BeginCopyPass();
			copyPass.UploadToBuffer(
				{ &m_transferBuffer, 0 },
				{ &m_buffer, 0, size },
				true
			);

			renderPass.BindVertexStorageBuffers(0, { &m_buffer });
			commandBuffer.PushVertexUniformData<Uniform>(0, uniform);
			renderPass.DrawPrimitives(rects.size() * 6, 1, 0, 0);
		}
	}

	void TextRenderLayer::Update(ECS& ecs, Window& window)
	{
		auto& renderers = ecs.GetComponents<TextRenderer>();
		auto size = renderers.size();
		if (m_indices.size() < size)
		{
			m_indices.reserve(size);
			for (size_t i = m_indices.size(); i < size; i++)
			{
				m_indices.push_back(i);
			}
		}
		else if (m_indices.size() > size)
		{
			m_indices.erase(std::find_if(m_indices.begin(), m_indices.end(), [size](size_t index)
				{
					return index >= size;
				}));
		}
		std::sort(m_indices.begin(), m_indices.end(), [&renderers](auto a, auto b)
			{
				auto layerA = renderers[a].layer;
				auto layerB = renderers[b].layer;
				return layerA < layerB;
			});

		uint32_t bufferSize = 0;
		for (auto& renderer : renderers)
		{
			bufferSize = std::max(bufferSize, static_cast<uint32_t>(renderer.GetRects().size()));
		}

		if (bufferSize != m_bufferSize)
		{
			uint32_t size = bufferSize * sizeof(Instance);
			m_buffer = window.CreateBuffer({ SDL::BufferUsage::GRAPHICS_STORAGE_READ, size });
			m_transferBuffer = window.CreateTransferBuffer({ SDL::TransferBufferUsage::UPLOAD, size });
			m_bufferSize = bufferSize;
		}
	}

	const std::vector<std::type_index>& TextRenderLayer::GetRendererTypes() const
	{
		return s_types;
	}
}

static const char* vertex = R"(
struct TextData
{
    float4 color;
    float texU, texV, texW, texH;
    int posX, posY, posW, posH;
};

StructuredBuffer<TextData> dataBuffer : register(t0, space0);

struct Output
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

static const uint indices[6] = { 0, 1, 2, 3, 2, 1 };

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 projection;
    float4x4 view;
    float4x4 model;
}


Output main(uint id : SV_VertexID)
{
    uint spriteIndex = id / 6;
    uint index = indices[id % 6];
    TextData data = dataBuffer[spriteIndex];
    
    const float2 texCoords[4] =
    {
        { data.texU,              data.texV },
        { data.texU + data.texW,  data.texV },
        { data.texU,              data.texV + data.texH },
        { data.texU + data.texW,  data.texV + data.texH }
    };
    
    const float2 positions[4] =
    {
        { data.posX,             data.posY },
        { data.posX + data.posW, data.posY },
        { data.posX,             data.posY + data.posH },
        { data.posX + data.posW, data.posY + data.posH }
    };
    
    Output output;
    output.position = mul(projection, mul(view, mul(model, float4(positions[index], 0.0f, 1.0f))));
    output.texCoord = texCoords[index];
    output.color = data.color;
    return output;
}
)";
static const char* fragment = R"(
Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

struct Input
{
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

float4 main(Input input) : SV_Target0
{
	const float smoothing = 1.0 / 16.0;
	float distance = Texture.Sample(Sampler, input.texCoord).a;
	float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
	return float4(input.color.rgb, input.color.a * alpha);
}
)";