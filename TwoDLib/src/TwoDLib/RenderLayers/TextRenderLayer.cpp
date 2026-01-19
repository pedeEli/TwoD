#include "tdpch.hpp"
#include "TextRenderLayer.hpp"

#include "TwoD/ECS/Transform.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Components/Camera.hpp"

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

		int width, height;
		App::Get<Window>().GetSize(width, height);

		auto camera = Camera::Get();
		Uniform uniformInWorld{
			camera->GetProjectionMatrix(),
			camera->GetWorldToCameraMatrix(),
			{ 1.0f }
		};
		Uniform uniformOnScreen{
			camera->GetProjectionMatrixFixedZoom(),
			{ 1.0f },
			{ 1.0f }
		};

		size_t i = 0;
		auto data = m_transferBuffer.Map<Instance>(true);
		for (auto index : m_indices)
		{
			auto& renderer = renderers[index];
			renderer.font->Bind(&renderPass);
			const auto& glyphs = renderer.GetGlyphs();

			for (size_t i = 0; i < glyphs.size(); i++)
			{
				auto& glyph = glyphs[i];
				data[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
				data[i].texMin = glyph.texMin;
				data[i].texMax = glyph.texMax;
				data[i].quadMin = glyph.quadMin;
				data[i].quadMax = glyph.quadMax;
			}

			auto size = static_cast<uint32_t>(sizeof(Instance) * glyphs.size());
			auto copyPass = commandBuffer.BeginCopyPass();
			copyPass.UploadToBuffer(
				{ &m_transferBuffer, 0 },
				{ &m_buffer, 0, size },
				true
			);

			Uniform& uniform = renderer.renderLocation == RenderLocation::InWorld ? uniformInWorld : uniformOnScreen;

			auto& model = renderer.GetComponent<Transform>()->GetWorldMatrix();
			uniform.model[0][0] = model[0][0];
			uniform.model[0][1] = model[0][1];
			uniform.model[1][0] = model[1][0];
			uniform.model[1][1] = model[1][1];
			uniform.model[3][0] = model[2][0];
			uniform.model[3][1] = model[2][1];

			renderPass.BindVertexStorageBuffers(0, { &m_buffer });
			commandBuffer.PushVertexUniformData<Uniform>(0, uniform);
			renderPass.DrawPrimitives(glyphs.size() * 6, 1, 0, 0);
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
			bufferSize = std::max(bufferSize, static_cast<uint32_t>(renderer.GetGlyphs().size()));
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
struct Instance
{
    float4 color;
    float2 texMin;
    float2 texMax;
    float2 quadMin;
    float2 quadMax;
};

StructuredBuffer<Instance> dataBuffer : register(t0, space0);

struct Output
{
    float4 position : SV_POSITION;
    float4 color : TEXCOORD0;
    float2 texCoord : TEXCOORD1;
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
    Instance data = dataBuffer[spriteIndex];
    
    const float2 texCoords[4] =
    {
        data.texMin,
        { data.texMax.x, data.texMin.y },
        { data.texMin.x, data.texMax.y },
        data.texMax
    };
    
    const float2 positions[4] =
    {
        data.quadMin,
        { data.quadMax.x, data.quadMin.y },
        { data.quadMin.x, data.quadMax.y },
        data.quadMax
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
    float4 color : TEXCOORD0;
    float2 texCoord : TEXCOORD1;
};

const static float pxRange = 4.0;

float2 sqr(float2 x)
{
	return x*x;
}

float screenPxRange(float2 texCoord)
{
    const float2 unitRange = float2(pxRange, pxRange) / float2(270.0f, 270.0f);
    // If inversesqrt is not available, use vec2(1.0)/sqrt
    const float2 screenTexSize = rsqrt(sqr(ddx(texCoord)) + sqr(ddy(texCoord)));
    // Can also be approximated as screenTexSize = vec2(1.0)/fwidth(texCoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float4 main(Input input) : SV_TARGET0
{
	//return float4(Texture.Sample(Sampler, input.texCoord).agb, 1.0f);
    const float3 msd = Texture.Sample(Sampler, input.texCoord).agb;
    const float sd = median(msd.r, msd.g, msd.b);
    const float screenPxDistance = screenPxRange(input.texCoord) * (sd - 0.5);
    const float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    return float4(input.color.rgb, input.color.a * opacity);
}
)";