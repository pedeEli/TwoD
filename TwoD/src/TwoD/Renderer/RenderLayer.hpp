#pragma once

#include <vector>
#include <typeindex>

#include "TwoD/ECS/ECS.hpp"
#include "TwoD/SDL/Window.hpp"
#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/SDL/CommandBuffer.hpp"

namespace TwoD
{
	class RenderPipeline;

	class RenderLayer
	{
	public:
		virtual ~RenderLayer() = default;
		virtual void Render(ECS& ecs, SDL::CommandBuffer& commandBuffer, SDL::RenderPass& renderPass) const = 0;
		virtual void Update(ECS& ecs, Window& window) = 0;

		virtual const std::vector<std::type_index>& GetRendererTypes() const = 0;

	private:
		bool m_dirty = true;

		friend class RenderPipeline;
	};
}