#pragma once
#include "TwoDLib.hpp"

using namespace TwoD;

class Player : public Component
{
	using Component::Component;
public:
	void Start() override;
	void Update(float delta) override;

public:
	TD_COMPONENT_FIELDS(
		TD_FIELD(float, speed, 1.5)
	)

	Camera* camera;
};

