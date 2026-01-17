#pragma once
#include <vector>
#include <typeindex>

#include "TwoD/Renderer/RenderLayer.hpp"
#include "TwoD/Renderer/Shader.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"
#include "TwoDLib/Assets/SpriteAtlas.hpp"

#include "TwoD/SDL/TransferBuffer.hpp"
#include "TwoD/SDL/Buffer.hpp"

namespace TwoD
{
	class TextRenderLayer : public RenderLayer
	{
	public:
		static std::optional<Shader> LoadShader();

	public:
		TextRenderLayer();
		~TextRenderLayer() = default;

		virtual void Render(ECS& ecs, SDL::CommandBuffer& commandBuffer, SDL::RenderPass& renderPass) const override;
		virtual void Update(ECS& ecs, Window& window) override;

		virtual const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		Shader m_shader;
		std::vector<size_t> m_indices;
		SDL::TransferBuffer m_transferBuffer;
		SDL::Buffer m_buffer;
		uint32_t m_bufferSize = 0;

		struct Instance
		{
			glm::fvec4 color;
			glm::fvec2 texMin;
			glm::fvec2 texMax;
			glm::fvec2 quadMin;
			glm::fvec2 quadMax;
		};
		struct Uniform
		{
			glm::fmat4x4 projection;
			glm::fmat4x4 view;
			glm::fmat4x4 model;
		};

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(TextRenderer) };
	};
}