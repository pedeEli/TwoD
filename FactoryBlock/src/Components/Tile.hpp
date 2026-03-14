#pragma once
#include "TwoD.hpp"
#include "TwoDLib.hpp"

TD_ENUM(
	(TD_NAME(Direction), TD_BASE(uint8_t)),
	(
		TD_ENUM_FIELD(NONE, TD_INIT(0)),
		TD_ENUM_FIELD(TOP, TD_INIT(1)),
		TD_ENUM_FIELD(RIGHT, TD_INIT(2)),
		TD_ENUM_FIELD(BOTTOM, TD_INIT(4)),
		TD_ENUM_FIELD(LEFT, TD_INIT(8))
	)
)

TD_COMPONENT(
	(TD_NAME(Tile)),
	(
		TD_COMPONENT_FIELD(Direction, directions, TD_INIT({})),
		TD_COMPONENT_FIELD(float, rotationSpeed, TD_INIT(20.0f))
	)
)
public:
	void Start() override;
	void Update(float delta) override;

public:
	float targetRotation = 0.0f;

private:
	void AddPath(const char* name, Direction dir);
};

