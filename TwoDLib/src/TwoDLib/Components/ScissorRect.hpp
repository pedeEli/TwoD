#pragma once
#include "TwoD/ECS/ECS.hpp"
#include "TwoD/Core/App.hpp"
#include "TwoD/Math/Rect.hpp"

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

