#pragma once
#include "TwoD.hpp"

TD_COMPONENT(
	(TD_NAME(Vein)),
	(
		TD_COMPONENT_FIELD(size_t, size)
	)
)
public:
	void Start() override;
};

