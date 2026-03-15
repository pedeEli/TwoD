#pragma once
#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/SDL/CommandBuffer.hpp"
#include "TwoD/SDL/Buffer.hpp"
#include "TwoD/SDL/TransferBuffer.hpp"
#include "TwoD/SDL/Texture.hpp"
#include "TwoD/SDL/Sampler.hpp"
#include "TwoD/ECS/ECS.hpp"
#include "TwoD/Math/Rect.hpp"

TD_STRUCT(
	(TD_NAME(Vertex), TD_NAMESPACE(TwoD)),
	(
		TD_STRUCT_FIELD(glm::fvec2, pos, TD_INIT({ 0.0f, 0.0f })),
		TD_STRUCT_FIELD(glm::fvec2, tex, TD_INIT({ 0.0f, 0.0f })),
		TD_STRUCT_FIELD(glm::fvec4, color, TD_INIT({ 1.0f, 1.0f, 1.0f, 1.0f }))
	),
)

namespace TwoD
{
	class RenderSystem;
	class RenderHandler;

	struct RenderHandlerInfo
	{
		size_t handlerIndex;
		size_t rendererIndex;
		int32_t layer;
		const glm::fmat4x4* projection;
		std::optional<Rect<float>> scissorRect;

		bool operator<(const RenderHandlerInfo& other) const noexcept
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

		void RenderMesh(
			const glm::fmat3x3& transform,
			const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices,
			const glm::fvec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }
		);
		void RenderMesh(
			const glm::fmat3x3& transform,
			const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices,
			const TextureBinding& binding,
			const glm::fvec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }
		);

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
			const std::vector<RenderHandlerInfo>& infos,
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
		struct RenderCommand
		{
			size_t handlerIndex = 0;
			size_t vertexStartIndex = 0;
			size_t vertexSize = 0;
			size_t indexStartIndex = 0;
			size_t indexSize = 0;
			const glm::fmat4x4* projection;
			std::optional<Rect<float>> scissorRect;
		};

	private:
		SDL::Buffer m_vertexBuffer;
		SDL::TransferBuffer m_vertexTransferBuffer;
		Vertex* m_vertexBufferPtr = nullptr;
		size_t m_vertexBufferIndex = 0;
		SDL::Buffer m_indexBuffer;
		SDL::TransferBuffer m_indexTransferBuffer;
		uint32_t* m_indexBufferPtr = nullptr;
		size_t m_indexBufferIndex = 0;

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
		static inline constexpr size_t s_maxNumberOfVertices = 16000;
		static inline constexpr size_t s_maxNumberOfIndices = 24000;

		friend class RenderSystem;
	};
}