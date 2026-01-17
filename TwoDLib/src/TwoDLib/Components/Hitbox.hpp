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
		TD_COMPONENT_FIELDS(
			TD_FIELD(float, x),
			TD_FIELD(float, y),
			TD_FIELD(float, w),
			TD_FIELD(float, h),
			TD_FIELD(int32_t, layer)
		)

	};
}