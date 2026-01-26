#pragma once

#include "TwoD/ECS/ECS.hpp"
#include "TwoDLib/Defines.hpp"

namespace TwoD
{
	class ColorRenderer : public Component
	{
		using Component::Component;
	public:
		void StartBefore() override;
		void Destroy() override;

	public:
		TD_COMPONENT_FIELDS(
			TD_FIELD(int32_t, layer, 0),
			TD_FIELD(RenderLocation, renderLocation, RenderLocation::InWorld),
			TD_FIELD(uint8_t, r),
			TD_FIELD(uint8_t, g),
			TD_FIELD(uint8_t, b),
			TD_FIELD(uint8_t, a)
		)
	};
}