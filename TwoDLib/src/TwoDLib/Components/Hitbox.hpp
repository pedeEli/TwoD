#pragma once
#include <optional>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "TwoD/Core/App.hpp"
#include "Camera.hpp"

namespace TwoD
{
	class Hitbox : public Component
	{
		using Component::Component;
	public:
		static std::optional<const Hitbox*> Hit(const glm::fvec2& pos);

	public:
		bool IsInside(const glm::fvec2& pos) const;
	
	public:
		TD_COMPONENT(
			TD_COMPONENT_FIELD(float, x),
			TD_COMPONENT_FIELD(float, y),
			TD_COMPONENT_FIELD(float, w),
			TD_COMPONENT_FIELD(float, h),
			TD_COMPONENT_FIELD(int32_t, layer)
		)

	};
}