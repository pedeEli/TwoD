#pragma once
#include "TwoD.hpp"
#include "TwoD/Renderer/RenderHandler.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "DefaultRenderHandler.hpp"

namespace TwoD
{
	class TextRenderHandler : public DefaultRenderHandler<TextRenderer>
	{
	public:
		void Init() override;

		void Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const override;
		void Render(Renderer& renderer, size_t index) override;
		
	private:
		Shader* m_shader = nullptr;
	};
}