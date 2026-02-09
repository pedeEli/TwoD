#pragma once
#include <typeindex>
#include "TwoD/SDL/CommandBuffer.hpp"
#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/ECS/ECS.hpp"

#include "Renderer.hpp"

namespace TwoD
{
	class RenderHandler
	{
	public:
		virtual ~RenderHandler() = default;

		virtual void Init() = 0;
		virtual void Update(size_t handlerIndex) = 0;
		virtual void Render(Renderer& renderer, size_t index) = 0;
		virtual void Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const = 0;

		virtual const std::vector<std::type_index>& GetRendererTypes() const = 0;

		template<class T>
		requires(std::is_base_of_v<Component, T>)
		std::vector<T>& GetComponents() const
		{
			return ECS::GetComponents<T>();
		}

	protected:
		std::vector<RendererHandlerInfo> m_rendererInfos;

		friend class RenderSystem;
	};
}

