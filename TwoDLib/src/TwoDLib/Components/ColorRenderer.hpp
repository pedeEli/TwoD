#pragma once
#include "TwoD.hpp"

namespace TwoD
{
	class ColorRenderer : public Component
	{
		using Component::Component;
	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD(int32_t, layer),
			TD_COMPONENT_FIELD(glm::u8vec4, color)
		)
	};
}