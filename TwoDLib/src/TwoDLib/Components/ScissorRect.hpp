#pragma once
#include "TwoD.hpp"

TD_COMPONENT(
	(TD_NAME(ScissorRect), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(Rect<float>, rect)
	)
)
	public:
		static void ComputeRect(Transform& transform);
	};
}

TD_COMPONENT(
	(TD_NAME(ComputedScissorRect), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(Rect<float>, rect)
	)
)
	};
}

