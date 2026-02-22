#pragma once
#include "TwoD.hpp"

namespace TwoD
{
	class Canvas : public Component
	{
		using Component::Component;
	public:
		void Start() override;
	};
}