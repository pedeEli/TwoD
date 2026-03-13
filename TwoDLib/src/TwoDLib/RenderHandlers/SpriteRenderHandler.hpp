#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Components/SpriteRenderer.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "TwoDLib/Assets/SpriteAtlas.hpp"
#include "DefaultRenderHandler.hpp"

namespace TwoD
{
	class SpriteRenderHandler : public DefaultRenderHandler<SpriteRenderer>
	{
	public:
		void Init() override;

		void Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const override;
		void Render(Renderer& renderer, size_t index) override;
		
	private:
		SpriteAtlas* m_spriteAtlas;
		Shader* m_shader;
	};
}