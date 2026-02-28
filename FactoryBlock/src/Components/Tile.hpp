#pragma once
#include "TwoD.hpp"

using namespace TwoD;

TD_YAML_ENUM(Direction,
	TD_YAML_ENUM_FIELD(TOP),
	TD_YAML_ENUM_FIELD(RIGHT),
	TD_YAML_ENUM_FIELD(BOTTOM),
	TD_YAML_ENUM_FIELD(LEFT)
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

