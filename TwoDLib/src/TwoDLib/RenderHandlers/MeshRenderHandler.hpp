#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Components/MeshRenderer.hpp"
#include "TwoDLib/Assets/Shader.hpp"
#include "DefaultRenderHandler.hpp"

namespace TwoD
{
	class MeshRenderHandler : public DefaultRenderHandler<MeshRenderer>
	{
	public:
		void Init() override;

		void Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const override;
		void Render(Renderer& renderer, size_t index) override;

	private:
		Shader* m_shader;
	};
}