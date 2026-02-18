#pragma once
#include <vector>
#include <typeindex>

#include "TwoD/Renderer/RenderHandler.hpp"
#include "TwoDLib/Components/TextRenderer.hpp"
#include "TwoDLib/Assets/Shader.hpp"

namespace TwoD
{
	class TextRenderHandler : public RenderHandler
	{
	public:
		void Init() override;

		void Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const override;
		void Render(Renderer& renderer, size_t index) override;
		void Update(size_t handlerIndex) override;

		const std::vector<std::type_index>& GetRendererTypes() const override;

	private:
		Shader* m_shader;

	private:
		static inline const std::vector<std::type_index> s_types = { typeid(TextRenderer) };
	};
}