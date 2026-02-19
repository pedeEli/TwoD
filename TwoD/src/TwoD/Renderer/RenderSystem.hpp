#pragma once
#include <typeindex>
#include "TwoD/ECS/ECS.hpp"
#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/SDL/CommandBuffer.hpp"
#include "TwoD/SDL/Buffer.hpp"
#include "TwoD/SDL/TransferBuffer.hpp"
#include "TwoD/SDL/Texture.hpp"
#include "TwoD/SDL/Sampler.hpp"
#include "TwoD/SDL/Shader.hpp"
#include "TwoD/SDL/GraphicsPipeline.hpp"
#include "TwoD/SDL/Fence.hpp"
#include "Renderer.hpp"
#include "RenderHandler.hpp"

namespace TwoD
{
	class RenderSystem
	{
	public:
		struct TextureBinding
		{
			SDL::TextureSamplerBinding* binding;
			uint32_t slot = 0;
		};

	public:
		RenderSystem() = default;
		~RenderSystem() = default;
		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;
		RenderSystem& operator=(RenderSystem&&) = delete;

		void Init(const Window& window);
		void Render(const Window& window);

		template<class Renderer>
		requires(std::is_base_of_v<Component, Renderer>)
		void UpdateLayerFor(int32_t layer)
		{
			m_dirty = true;
		}

		template<class Handler>
		requires(std::is_base_of_v<RenderHandler, Handler>)
		void RegisterHandler()
		{
			TD_CORE_ASSERT(std::all_of(m_renderHandlers.begin(), m_renderHandlers.end(), [](std::unique_ptr<RenderHandler>& handler)
				{
					return dynamic_cast<Handler*>(handler.get()) == nullptr;
				}), "Cannot register RenderHandler twice!");

			auto handler = std::make_unique<Handler>();
			const auto& types = handler->GetRendererTypes();
			auto index = m_renderHandlers.size();
			m_renderHandlers.push_back(std::move(handler));

			for (const auto& type : types)
			{
				auto it = m_renderersToHandlers.find(type);
				if (it == m_renderersToHandlers.end())
				{
					m_renderersToHandlers[type] = { index };
					continue;
				}
				it->second.push_back(index);
			}
		}

	private:
		void Update();
		void CreateTargetTexture(uint32_t width, uint32_t height);

	private:
		Renderer m_renderer;
		std::vector<std::unique_ptr<RenderHandler>> m_renderHandlers;
		std::unordered_map<std::type_index, std::vector<size_t>> m_renderersToHandlers;
		std::vector<RendererHandlerInfo> m_rendererHandlerInfos;
		bool m_dirty = true;
		SDL::Fence m_fence;

		SDL::Texture m_targetTexture;
		SDL::Sampler m_targetSampler;
		SDL::Shader m_quadVertexShader;
		SDL::Shader m_quadFragmentShader;
		SDL::GraphicsPipeline m_quadPipeline;
	};
}