#pragma once
#include <typeindex>
#include "TwoD/SDL/CommandBuffer.hpp"
#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/ECS/ECS.hpp"
#include "TwoD/Math/Rect.hpp"

#include "Renderer.hpp"

namespace TwoD
{
	class RenderHandler
	{
	public:
		RenderHandler() = default;
		virtual ~RenderHandler() = default;
		RenderHandler(const RenderHandler& other) = delete;
		RenderHandler(RenderHandler&& other) = delete;
		RenderHandler& operator=(const RenderHandler& other) = delete;
		RenderHandler& operator=(RenderHandler&& other) = delete;

		virtual void Init() = 0;
		virtual void Render(Renderer& renderer, size_t index) = 0;
		virtual void Bind(const SDL::CommandBuffer* commandBuffer, const SDL::RenderPass* renderPass) const = 0;
		virtual void Update(size_t handlerIndex) = 0;

		template<class T>
		requires(std::is_base_of_v<Component, T>)
		std::vector<T>& GetComponents() const
		{
			return ECS::GetComponents<T>();
		}

	protected:
		std::vector<RenderHandlerInfo> m_rendererInfos;

		friend class RenderSystem;
		friend class Renderer;
	};
}

