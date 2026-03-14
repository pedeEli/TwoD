#pragma once
#include "TwoD.hpp"
#include "TwoDLib.hpp"


TD_COMPONENT(
	(TD_NAME(Player)),
	(
		TD_COMPONENT_FIELD(float, speed, TD_INIT(1.5f))
	)
)
public:
	void Start() override;
	void Update(float delta) override;

	TwoD::Camera* camera;
};

