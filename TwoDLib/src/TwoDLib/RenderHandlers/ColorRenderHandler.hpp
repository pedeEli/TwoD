#pragma once
#include "TwoD/Renderer/RenderHandler.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"

namespace TwoD
{
	class ColorRenderHandler : public RenderHandler
	{
	public:
		void Init() override;

		void Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const override;
		void Render(const ECS& ecs, Renderer& renderer, size_t index) override;
		void Update(const ECS& ecs, size_t handlerIndex) override;

		const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		Shader* m_shader;

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(ColorRenderer) };
	};
}