#pragma once

#include "TwoD/ECS/ECS.hpp"
#include "TwoDLib/Assets/Sprite.hpp"

namespace TwoD
{
	class SpriteRenderer : public Component
	{
		using Component::Component;
	public:
		void StartBefore() override;
		void Destroy() override;

	public:
		TD_COMPONENT_FIELDS(
			TD_FIELD(int32_t, layer, 0),
			TD_FIELD(Sprite*, sprite),
			TD_FIELD(std::optional<uint32_t>, slice)
		)
	};
}