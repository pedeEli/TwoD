#pragma once
#include <vector>
#include <typeindex>

#include "TwoD/Renderer/RenderLayer.hpp"
#include "TwoDLib/Components/SpriteRenderer.hpp"
#include "TwoD/Renderer/Shader.hpp"
#include "TwoDLib/Assets/SpriteAtlas.hpp"

#include "TwoD/SDL/TransferBuffer.hpp"
#include "TwoD/SDL/Buffer.hpp"

namespace TwoD
{
	class SpriteRenderLayer : public RenderLayer
	{
	public:
		static std::optional<Shader> LoadShader();

	public:
		SpriteRenderLayer();
		~SpriteRenderLayer() = default;

		virtual void Render(ECS& ecs, SDL::CommandBuffer& commandBuffer, SDL::RenderPass& renderPass) const override;
		virtual void Update(ECS& ecs, Window& window) override;

		virtual const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		Shader m_shader;
		std::vector<size_t> m_indices;
		SDL::TransferBuffer m_transferBuffer;
		SDL::Buffer m_buffer;

		struct Instance
		{
			glm::fvec4 color;
			SpriteRect tex;
			glm::fvec2 model1;
			glm::fvec2 model2;
			glm::fvec2 model3;
			glm::fvec2 padding;
		};
		struct Uniform
		{
			glm::fmat4x4 projection;
			glm::fmat4x4 view;
			glm::fvec2 atlasSize;
		};

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(SpriteRenderer) };
	};
}