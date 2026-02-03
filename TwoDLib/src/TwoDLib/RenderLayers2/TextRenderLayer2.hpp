#pragma once
#include <vector>
#include <typeindex>

#include "TwoD/Renderer2/RenderLayer2.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"
#include "TwoDLib/Assets/Shader.hpp"

namespace TwoD
{
	class TextRenderLayer2 : public RenderLayer2
	{
	public:
		void Init() override;

		void Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const override;
		void Render(const ECS& ecs, RenderSystem2& renderSystem, SDL::RenderPass* renderPass, size_t index) override;
		void Update(const ECS& ecs) override;

		const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		Shader* m_shader;

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(TextRenderer) };
	};
}