#pragma once
#include "TwoD.hpp"

using namespace TwoD;

template<typename T>
struct Field
{
	const char* name;
	T* ptr;
};

class Ground : public Component
{
	using Component::Component;
public:
	void Start() override;

public:
	TD_COMPONENT(
		TD_COMPONENT_FIELD(uint32_t, width),
		TD_COMPONENT_FIELD(uint32_t, height),
		TD_COMPONENT_FIELD(uint32_t, foliage)
	)
};