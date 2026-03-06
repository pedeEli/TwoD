#pragma once
#include "TwoD.hpp"

namespace TwoD
{
	class ScissorRect : public Component
	{
		using Component::Component;
	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD(Rect<float>, rect)
		)
	};
}

