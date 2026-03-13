#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Assets/Sprite.hpp"

namespace TwoD
{
	class SpriteRenderer : public Component
	{
		using Component::Component;
	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD(int32_t, layer),
			TD_COMPONENT_FIELD(Sprite*, sprite),
			TD_COMPONENT_FIELD(std::optional<uint32_t>, slice, {})
		)
	};
}