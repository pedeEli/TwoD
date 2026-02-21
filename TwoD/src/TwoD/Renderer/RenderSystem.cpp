#include "tdpch.hpp"
#include "RenderSystem.hpp"

#include "TwoD/Core/App.hpp"
#include "TwoD/Debug/Debug.hpp"

static const char* s_quadVertexCode = R"(
struct Output
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

static const float2 positions[3] = {
    float2(-1.0, -1.0),
    float2(-1.0,  3.0),
    float2( 3.0, -1.0)
};

static const float2 uvs[3] = {
    float2(0.0, 1.0),
    float2(0.0, -1.0),
    float2(2.0, 1.0)
};

Output main(uint id: SV_VertexID)
{
    Output output;
    output.pos = float4(positions[id], 0.0, 1.0);
    output.uv = uvs[id];
    return output;
})";

static const char* s_quadFragmentCode = R"(
Texture2D<float4> texture : register(t0, space2);
SamplerState samplr : register(s0, space2);

float4 main(float2 uv : TEXCOORD0) : SV_Target0
{
    return texture.Sample(samplr, uv);
})";

namespace TwoD
{
	static SDL::GraphicsPipeline CreateQuadPipeline(const Window& window, SDL::Shader* vertex, SDL::Shader* fragment)
	{
		SDL::ColorTargetDescription colorTarget{
			.format = window.GetSwapchainTextureFormat()
		};
		SDL::GraphicsPipelineInfo pipelineInfo{
			.vertexShader = vertex,
			.fragmentShader = fragment,
			.primitiveType = SDL::PrimitiveType::TRIANGLELIST,
			.targetInfo = {
				.colorTargetDescriptions = { colorTarget }
			}
		};
		return window.CreateGraphicsPipeline(pipelineInfo);
	}
	static SDL::Sampler CreateTargetSampler(const Window& window)
	{
		return window.CreateSampler({
			.minFilter = SDL::Filter::LINEAR,
			.magFilter = SDL::Filter::LINEAR,
			.mipmapMode = SDL::SamplerMipmapMode::LINEAR,
			.addressModeU = SDL::SamplerAddressMode::CLAMP_TO_EDGE,
			.addressModeV = SDL::SamplerAddressMode::CLAMP_TO_EDGE,
			.addressModeW = SDL::SamplerAddressMode::CLAMP_TO_EDGE
		});
	}

	void RenderSystem::Init(const Window& window)
	{
		for (auto& renderLayer : m_renderHandlers)
		{
			renderLayer->Init();
		}

		m_renderer.Init(window);

		int width, height;
		window.GetSize(width, height);
		CreateTargetTexture(width, height);
		EventHandler::On<WindowResizedEvent>([this](auto& e)
			{
				CreateTargetTexture(e.x, e.y);
				return false;
			});
		m_targetSampler = CreateTargetSampler(window);

		m_quadVertexShader = SDL::Shader::Load(window, s_quadVertexCode, TwoD::SDL::ShaderStage::VERTEX, 0, 0, 0, 0);
		m_quadFragmentShader = SDL::Shader::Load(window, s_quadFragmentCode, TwoD::SDL::ShaderStage::FRAGMENT, 1, 0, 0, 0);
		m_quadPipeline = CreateQuadPipeline(window, &m_quadVertexShader, &m_quadFragmentShader);
	}

	void RenderSystem::Shutdown()
	{
		m_renderer.Shutdown();
		m_targetTexture.Release();
		m_targetSampler.Release();
		m_quadVertexShader.Release();
		m_quadFragmentShader.Release();
		m_quadPipeline.Release();
		m_fence.Release();
	}

	void RenderSystem::CreateTargetTexture(uint32_t width, uint32_t height)
	{
		auto& window = App::Get<Window>();
		m_targetTexture.Release();
		m_targetTexture = window.CreateTexture({
			.type = SDL::TextureType::TWO_D,
			.format = window.GetSwapchainTextureFormat(),
			.usage = SDL::TextureUsageFlags::COLOR_TARGET | SDL::TextureUsageFlags::SAMPLER,
			.width = width,
			.height = height,
			.layerCountOrDepth = 1,
			.numLevels = 1,
			.sampleCount = SDL::SampleCount::ONE
		});
	}

	void RenderSystem::Update(const Window& window)
	{
		auto commandBuffer = window.AcquireCommandBuffer();
		
		if (Debug::GameIsRunning())
		{
			if (m_dirty)
			{
				SortLayers();
				m_dirty = false;
			}

			m_fence.Wait();
			auto renderPass = window.BeginRenderPass(&commandBuffer, &m_targetTexture);
			m_renderer.Render(commandBuffer, renderPass, m_rendererHandlerInfos, &m_renderHandlers);
			renderPass.End();
		}

		auto renderPass = window.BeginRenderPass(&commandBuffer, nullptr);
		if (!renderPass)
		{
			TD_CORE_ERROR("Invalid render pass");
		}
		else
		{
			renderPass.BindGraphicsPipeline(&m_quadPipeline);
			renderPass.BindFragmentSamplers(0, { { &m_targetTexture, &m_targetSampler } });
			renderPass.DrawPrimitives(3, 1, 0, 0);

			Debug::Render(commandBuffer, renderPass);
		}
		renderPass.End();

		m_fence.Release();
		m_fence = commandBuffer.Submit();
	}

	void RenderSystem::SortLayers()
	{
		size_t totalSize = 0;
		for (size_t i = 0; i < m_renderHandlers.size(); i++)
		{
			auto& handler = m_renderHandlers[i];
			handler->Update(i);
			totalSize += handler->m_rendererInfos.size();
		}

		m_rendererHandlerInfos.clear();
		m_rendererHandlerInfos.reserve(totalSize);
		std::vector<RendererHandlerInfo> temp;
		temp.reserve(totalSize);

		for (auto& handler : m_renderHandlers)
		{
			std::merge(
				handler->m_rendererInfos.begin(), handler->m_rendererInfos.end(),
				m_rendererHandlerInfos.begin(), m_rendererHandlerInfos.end(),
				std::back_inserter(temp)
			);
			std::swap(m_rendererHandlerInfos, temp);
			temp.clear();
		}
	}
}