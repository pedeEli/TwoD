#pragma once
#include "TwoD.hpp"
#include "TwoDLib/Assets/Mesh.hpp"

TD_COMPONENT(
	(TD_NAME(MeshRenderer), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(int32_t, layer),
		TD_COMPONENT_FIELD(Mesh*, mesh, TD_INIT(nullptr)),
		TD_COMPONENT_FIELD(glm::u8vec4, color, TD_INIT({ 255, 255, 255, 255 }))
	)
)
	};
}

