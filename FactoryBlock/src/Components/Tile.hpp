#pragma once
#include "TwoD.hpp"
#include "TwoDLib.hpp"

using namespace TwoD;

TD_ENUM(
	(TD_NAME(Direction)),
	(
		TD_ENUM_FIELD(TOP),
		TD_ENUM_FIELD(RIGHT),
		TD_ENUM_FIELD(BOTTOM),
		TD_ENUM_FIELD(LEFT)
	)
)

class Tile : public Component
{
	using Component::Component;
public:
	void Start() override;
	void Update(float delta) override;

	TD_COMPONENT(
		TD_COMPONENT_FIELD(std::vector<Direction>, directions, {}),
		TD_COMPONENT_FIELD(float, rotationSpeed, 20.0f)
	)

public:
	float targetRotation = 0.0f;

private:
	void AddPath(const char* name, Direction dir);
};

