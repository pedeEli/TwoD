#include "tdpch.hpp"
#include "SpriteRenderLayer.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoD/Components/Camera.hpp"

extern const char* vertex;
extern const char* fragment;

namespace TwoD
{
	std::optional<Shader> SpriteRenderLayer::LoadShader()
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
			TD_CORE_ERROR("Failed to load sprite vertex shader");
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
			TD_CORE_ERROR("Failed to load sprite fragment shader");
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

	SpriteRenderLayer::SpriteRenderLayer()
	{
		auto shader = LoadShader();
		if (shader)
		{
			m_shader = std::move(*shader);
		}
		App::Get<AssetManager>().Get<SpriteAtlas>("sprite-atlas").Pack();
	}

	void SpriteRenderLayer::Render(ECS& ecs, SDL::CommandBuffer& commandBuffer, SDL::RenderPass& renderPass) const
	{
		auto& renderers = ecs.GetComponents<SpriteRenderer>();
		auto size = static_cast<uint32_t>(sizeof(Instance) * renderers.size());

		auto data = m_transferBuffer.Map<Instance>(true);
		for (size_t i = 0; i < m_indices.size(); i++)
		{
			auto& renderer = renderers[m_indices[i]];

			auto& model = renderer.GetComponent<Transform>()->GetWorldMatrix();
			data[i].model1.x = model[0].x;
			data[i].model1.y = model[0].y;
			data[i].model2.x = model[1].x;
			data[i].model2.y = model[1].y;
			data[i].model3.x = model[2].x;
			data[i].model3.y = model[2].y;

			data[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			if (renderer.slice)
			{
				data[i].tex = renderer.sprite->GetRect(*renderer.slice);
			}
			else
			{
				data[i].tex = renderer.sprite->GetRect();
			}
		}

		if (size == 0)
		{
			return;
		}

		auto copyPass = commandBuffer.BeginCopyPass();
		copyPass.UploadToBuffer(
			{ &m_transferBuffer, 0 },
			{ &m_buffer, 0, size },
			true
		);

		m_shader.Bind(&renderPass);
		renderPass.BindVertexStorageBuffers(0, { &m_buffer });
		auto& atlas = App::Get<AssetManager>().Get<SpriteAtlas>("sprite-atlas");
		atlas.Bind(&renderPass);

		auto camera = Camera::Get();
		Uniform uniform{
			camera->GetProjectionMatrix(),
			camera->GetWorldToCameraMatrix(),
			atlas.GetSize()
		};
		commandBuffer.PushVertexUniformData<Uniform>(0, uniform);

		renderPass.DrawPrimitives(static_cast<uint32_t>(renderers.size() * 6), 1, 0, 0);
	}

	void SpriteRenderLayer::Update(ECS& ecs, Window& window)
	{
		auto& renderers = ecs.GetComponents<SpriteRenderer>();
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

		auto bufferSize = static_cast<uint32_t>(sizeof(Instance) * size);
		m_buffer = window.CreateBuffer({ SDL::BufferUsage::GRAPHICS_STORAGE_READ, bufferSize });
		m_transferBuffer = window.CreateTransferBuffer({ SDL::TransferBufferUsage::UPLOAD, bufferSize });
	}

	const std::vector<std::type_index>& SpriteRenderLayer::GetRendererTypes() const
	{
		return s_types;
	}
}

static const char* vertex = R"(
struct SpriteData
{
    float4 color;
    float texU, texV, texW, texH;
    float2 model1;
    float2 model2;
    float2 model3;
    float2 padding;
};

StructuredBuffer<SpriteData> dataBuffer : register(t0, space0);

struct Output
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
};

static const uint indices[6] = { 0, 1, 2, 3, 2, 1 };
static const float3 positions[4] =
{
    { -0.5f, -0.5f, 1.0f },
    {  0.5f, -0.5f, 1.0f },
    { -0.5f,  0.5f, 1.0f },
    {  0.5f,  0.5f, 1.0f }
};

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 projection;
    float4x4 view;
    float2 atlasSize;
}


Output main(uint id : SV_VertexID)
{
    uint spriteIndex = id / 6;
    uint index = indices[id % 6];
    SpriteData data = dataBuffer[spriteIndex];
    
    float texelOffsetU = 0.5f / atlasSize.x;
    float texelOffsetV = 0.5f / atlasSize.y;
    
    float2 texCoords[4] = {
        { data.texU + texelOffsetU,              data.texV + texelOffsetV },
        { data.texU + data.texW - texelOffsetU,  data.texV + texelOffsetV },
        { data.texU + texelOffsetU,              data.texV + data.texH - texelOffsetV },
        { data.texU + data.texW - texelOffsetU,  data.texV + data.texH - texelOffsetV }
    };
    
    float3x3 model =
    {
      data.model1.x, data.model2.x, data.model3.x,  
      data.model1.y, data.model2.y, data.model3.y,
      0.0f,          0.0f,          1.0f
    };
    float2 position = mul(model, positions[index]).xy;
    
    Output output;
    output.position = mul(projection, mul(view, float4(position, 0.0f, 1.0f)));
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
    return input.color * Texture.Sample(Sampler, input.texCoord);
}
)";