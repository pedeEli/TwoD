#pragma once
#include "TwoD.hpp"

using namespace TwoD;

class Vein : public Component
{
	using Component::Component;
public:
	void Start() override;

public:
	TD_COMPONENT(
		TD_COMPONENT_FIELD(size_t, size)
	)
};

