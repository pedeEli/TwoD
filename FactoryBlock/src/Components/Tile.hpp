#pragma once
#include "TwoD.hpp"
#include "TwoDLib.hpp"

using namespace TwoD;

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

class Tile : public Component
{
	using Component::Component;
public:
	void Start() override;
	void Update(float delta) override;

	TD_COMPONENT(
		TD_COMPONENT_FIELD(Direction, directions, {}),
		TD_COMPONENT_FIELD(float, rotationSpeed, 20.0f)
	)

public:
	float targetRotation = 0.0f;

private:
	void AddPath(const char* name, Direction dir);
};

