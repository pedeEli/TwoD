#pragma once
#include <vector>
#include <typeindex>

#include "TwoD/Renderer/RenderHandler.hpp"
#include "TwoDLib/Components/SpriteRenderer.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Assets/SpriteAtlas.hpp"

namespace TwoD
{
	class SpriteRenderHandler : public RenderHandler
	{
	public:
		void Init() override;

		void Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const override;
		void Render(const ECS& ecs, Renderer& renderer, size_t index) override;
		void Update(const ECS& ecs, size_t handlerIndex) override;

		const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		SpriteAtlas* m_spriteAtlas;
		Shader* m_shader;

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(SpriteRenderer) };
	};
}