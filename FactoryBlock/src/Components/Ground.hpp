#pragma once
#include "TwoD.hpp"


TD_COMPONENT(
	(TD_NAME(Ground)),
	(
		TD_COMPONENT_FIELD(uint32_t, width),
		TD_COMPONENT_FIELD(uint32_t, height),
		TD_COMPONENT_FIELD(uint32_t, foliage)
	)
)
public:
	void Start() override;
};