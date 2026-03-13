#pragma once
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Components/ColorRenderer.hpp"
#include "DefaultRenderHandler.hpp"

namespace TwoD
{
	class ColorRenderHandler : public DefaultRenderHandler<ColorRenderer>
	{
	public:
		void Init() override;

		void Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const override;
		void Render(Renderer& renderer, size_t index) override;

	private:
		Shader* m_shader;
	};
}