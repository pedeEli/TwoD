#pragma once
#include <vector>
#include <typeindex>

#include "TwoD/Renderer/RenderLayer.hpp"
#include "TwoDLib/Components/SpriteRenderer.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Assets/SpriteAtlas.hpp"

namespace TwoD
{
	class SpriteRenderLayer : public RenderLayer
	{
	public:
		void Init() override;

		void Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const override;
		void Render(const ECS& ecs, RenderSystem& renderSystem, SDL::RenderPass* renderPass, size_t index) override;
		void Update(const ECS& ecs) override;

		const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		SpriteAtlas* m_spriteAtlas;
		Shader* m_shader;

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(SpriteRenderer) };
	};
}