#pragma once
#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/SDL/CommandBuffer.hpp"
#include "TwoD/SDL/Buffer.hpp"
#include "TwoD/SDL/TransferBuffer.hpp"
#include "TwoD/SDL/Texture.hpp"
#include "TwoD/SDL/Sampler.hpp"
#include "TwoD/ECS/ECS.hpp"
#include "TwoD/Math/Rect.hpp"

namespace TwoD
{
	class RenderSystem;
	class RenderHandler;

	struct RendererHandlerInfo
	{
		size_t handlerIndex;
		size_t rendererIndex;
		int32_t layer;
		const glm::fmat4x4* projection;
		const Rect<float>* scissorRect;

		bool operator<(const RendererHandlerInfo& other) const noexcept
		{
			if (other.layer == layer)
			{
				return projection < other.projection;
			}
			return layer < other.layer;
		}
	};

	class Renderer
	{
	public:
		struct TextureBinding
		{
			SDL::TextureSamplerBinding* binding;
			uint32_t slot = 0;
		};

	public:
		~Renderer() = default;
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) = delete;

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
		Renderer() = default;

		void Init(const Window& window);
		void Render(
			const SDL::CommandBuffer& commandBuffer,
			const SDL::RenderPass& renderPass,
			const std::vector<RendererHandlerInfo>& infos,
			const std::vector<std::unique_ptr<RenderHandler>>* handlers
		);
		void Shutdown();
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
		struct Vertex
		{
			glm::fvec2 pos;
			glm::fvec2 tex;
			glm::fvec4 color;
		};
		struct RenderCommand
		{
			size_t handlerIndex = 0;
			size_t startIndex = 0;
			size_t size = 0;
			const glm::fmat4x4* projection;
			const Rect<float>* scissorRect;
		};

	private:
		SDL::Buffer m_vertexBuffer;
		SDL::TransferBuffer m_vertexTransferBuffer;
		Vertex* m_vertexBufferPtr = nullptr;
		SDL::Buffer m_indexBuffer;
		SDL::TransferBuffer m_indexTransferBuffer;
		uint32_t* m_indexBufferPtr = nullptr;
		size_t m_quadIndex = 0;

		const SDL::CommandBuffer* m_commandBuffer = nullptr;
		const SDL::RenderPass* m_renderPass = nullptr;
		RenderCommand m_currentRenderCommand{};
		std::vector<RenderCommand> m_renderCommands;

		std::array<SDL::TextureSamplerBinding*, 4> m_textureBindings{};
		SDL::Texture m_dummyTexture;
		SDL::Sampler m_dummySampler;
		SDL::TextureSamplerBinding m_dummyBinding;

		const std::vector<std::unique_ptr<RenderHandler>>* m_handlers = nullptr;

	private:
		static inline constexpr size_t s_maxNumberOfQuads = 4000;
		static inline constexpr size_t s_maxNumberOfVertices = s_maxNumberOfQuads * 4;
		static inline constexpr size_t s_maxNumberOfIndices = s_maxNumberOfQuads * 6;

		friend class RenderSystem;
	};
}