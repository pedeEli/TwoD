#pragma once
#include <typeindex>
#include "TwoD/SDL/CommandBuffer.hpp"
#include "TwoD/SDL/RenderPass.hpp"
#include "TwoD/ECS/ECS.hpp"

namespace TwoD
{
	class RenderSystem2;

	class RenderLayer2
	{
	public:
		struct IndexLayer
		{
			size_t index;
			int32_t layer;
			const glm::fmat4x4* projection;
		};

	public:
		virtual ~RenderLayer2() = default;

		virtual void Init() {};
		virtual void Update(const ECS& ecs) = 0;
		virtual void Render(const ECS& ecs, RenderSystem2& renderSystem, SDL::RenderPass* renderPass, size_t index) = 0;
		virtual void Bind(SDL::CommandBuffer* commandBuffer, SDL::RenderPass* renderPass) const = 0;

		virtual const std::vector<std::type_index>& GetRendererTypes() const = 0;

	protected:
		std::vector<IndexLayer> m_indexLayers;

		friend class RenderSystem2;
	};
}

