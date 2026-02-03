#pragma once
#include <typeindex>
#include "TwoD/ECS/ECS.hpp"
#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/SDL/CommandBuffer.hpp"
#include "TwoD/SDL/Buffer.hpp"
#include "TwoD/SDL/TransferBuffer.hpp"
#include "TwoD/SDL/Texture.hpp"
#include "TwoD/SDL/Sampler.hpp"
#include "RenderLayer2.hpp"

namespace TwoD
{
	class RenderSystem2
	{
	public:
		struct TextureBinding
		{
			SDL::TextureSamplerBinding* binding;
			uint32_t slot = 0;
		};

	public:
		RenderSystem2() = default;
		~RenderSystem2() = default;
		RenderSystem2(const RenderSystem2&) = delete;
		RenderSystem2(RenderSystem2&&) = delete;
		RenderSystem2& operator=(const RenderSystem2&) = delete;
		RenderSystem2& operator=(RenderSystem2&&) = delete;

		void Init();
		void Render();

		template<class Renderer>
		requires(std::is_base_of_v<Component, Renderer>)
		void UpdateLayerFor(int32_t layer)
		{
			m_dirty = true;
		}

		template<class Layer>
		requires(std::is_base_of_v<RenderLayer2, Layer>)
		void RegisterLayer()
		{
			TD_CORE_ASSERT(std::all_of(m_renderLayers.begin(), m_renderLayers.end(), [](std::unique_ptr<RenderLayer2>& layer)
				{
					return dynamic_cast<Layer*>(layer.get()) == nullptr;
				}), "Cannot register RenderLayer twice!");

			auto layer = std::make_unique<Layer>();
			const auto& types = layer->GetRendererTypes();
			auto index = m_renderLayers.size();
			m_renderLayers.push_back(std::move(layer));

			for (const auto& type : types)
			{
				auto it = m_typesToLayers.find(type);
				if (it == m_typesToLayers.end())
				{
					m_typesToLayers[type] = { index };
					continue;
				}
				it->second.push_back(index);
			}
		}

		void RenderQuad(
			const glm::fmat3x3& transform,
			const glm::fvec2& pos,
			const glm::fvec2& size,
			const glm::fvec4& color
		);
		void RenderQuad(
			const glm::fmat3x3& transform,
			const glm::fvec2& pos,
			const glm::fvec2& size,
			const glm::fvec2& tex1,
			const glm::fvec2& tex2,
			const TextureBinding& binding,
			const glm::fvec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }
		);

	private:
		struct Index
		{
			size_t renderLayer;
			size_t index;
			int32_t layer;
			const glm::fmat4x4* projection;
		};
		struct Vertex
		{
			glm::fvec2 pos;
			glm::fvec2 tex;
			glm::fvec4 color;
		};
		struct RenderCommand
		{
			size_t renderLayer = 0;
			size_t startIndex = 0;
			size_t size = 0;
			const glm::fmat4x4* projection;
		};

	private:
		void Update(const ECS& ecs);
		void NextBatch();
		void RenderQuad(
			const glm::fmat3x3& transform,
			const glm::fvec2& pos,
			const glm::fvec2& size,
			const glm::fvec2& tex1,
			const glm::fvec2& tex2,
			const glm::fvec4& color
		);

	private:
		std::vector<std::unique_ptr<RenderLayer2>> m_renderLayers;
		std::unordered_map<std::type_index, std::vector<size_t>> m_typesToLayers;
		std::vector<Index> m_indices;
		bool m_dirty = true;
		
		SDL::Buffer m_vertexBuffer;
		SDL::TransferBuffer m_vertexTransferBuffer;
		Vertex* m_vertexBufferPtr;
		SDL::Buffer m_indexBuffer;
		SDL::TransferBuffer m_indexTransferBuffer;
		uint32_t* m_indexBufferPtr;
		size_t m_quadIndex = 0;

		SDL::CommandBuffer* m_commandBuffer;
		SDL::RenderPass* m_renderPass;
		RenderCommand m_currentRenderCommand{};
		std::vector<RenderCommand> m_renderCommands;

		std::array<SDL::TextureSamplerBinding*, 4> m_textureBindings{};
		SDL::Texture m_dummyTexture;
		SDL::Sampler m_dummySampler;
		SDL::TextureSamplerBinding m_dummyBinding;

	private:
		static inline constexpr size_t s_maxNumberOfQuads = 4000;
		static inline constexpr size_t s_maxNumberOfVertices = s_maxNumberOfQuads * 4;
		static inline constexpr size_t s_maxNumberOfIndices = s_maxNumberOfQuads * 6;
	};
}