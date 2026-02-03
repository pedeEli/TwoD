#pragma once
#include "TwoD/Renderer/RenderLayer.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"

namespace TwoD
{
	class ColorRenderLayer : public RenderLayer
	{
	public:
		void Init() override;

		void Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const override;
		void Render(const ECS& ecs, RenderSystem& renderSystem, SDL::RenderPass* renderPass, size_t index) override;
		void Update(const ECS& ecs) override;

		const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		Shader* m_shader;

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(ColorRenderer) };
	};
}