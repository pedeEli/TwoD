#pragma once
#include "TwoD/Renderer/RenderLayer.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"

namespace TwoD
{
	class ColorRenderLayer : public RenderLayer
	{
	public:
		ColorRenderLayer();

		virtual void Render(ECS& ecs, SDL::CommandBuffer& commandBuffer, SDL::RenderPass& renderPass) const override;
		virtual void Update(ECS& ecs, Window& window) override;

		virtual const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		Shader* m_shader;
		std::vector<size_t> m_indices;
		SDL::TransferBuffer m_transferBuffer;
		SDL::Buffer m_buffer;
		uint32_t m_bufferSize = 0;

		struct Instance
		{
			glm::fvec4 color;
			glm::fvec2 model1;
			glm::fvec2 model2;
			glm::fvec2 model3;
			bool inWorld;
			float padding;
		};
		struct Uniform
		{
			glm::fmat4x4 projectionWithZoom;
			glm::fmat4x4 projectionWithoutZoom;
			glm::fmat4x4 view;
		};

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(ColorRenderer) };
	};
}