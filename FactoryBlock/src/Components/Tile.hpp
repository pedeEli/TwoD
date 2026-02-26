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

private:
	void AddPath(const char* name, Direction dir);
};

