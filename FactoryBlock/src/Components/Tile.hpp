#pragma once
#include "TwoD.hpp"

using namespace TwoD;

class Tile : public Component
{
	using Component::Component;
	enum class Direction : uint8_t
	{
		TOP, RIGHT, BOTTOM, LEFT
	};
public:
	void Start() override;
	void Update(float delta) override;

	TD_COMPONENT(
		TD_COMPONENT_FIELD(float, rotationSpeed, 20.0f)
	)

public:
	float targetRotation = 0.0f;

private:
	void AddPath(const char* name, Direction dir);
};

