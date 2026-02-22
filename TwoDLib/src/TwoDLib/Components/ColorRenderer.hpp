#pragma once

#include "TwoD/ECS/ECS.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoDLib/Defines.hpp"

namespace TwoD
{
	class ColorRenderer : public Component
	{
		using Component::Component;
	public:
		void StartBefore() override;
		void Destroy() override;

		void SetLayer(uint32_t layer);

	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD_WITH_UPDATER(int32_t, layer, 0, SetLayer(layer);),
			TD_COMPONENT_FIELD(RenderLocation, renderLocation, RenderLocation::InWorld),
			TD_COMPONENT_FIELD(uint8_t, r),
			TD_COMPONENT_FIELD(uint8_t, g),
			TD_COMPONENT_FIELD(uint8_t, b),
			TD_COMPONENT_FIELD(uint8_t, a, 255)
		)
	};
}