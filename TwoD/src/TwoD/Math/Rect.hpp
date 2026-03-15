#pragma once
#include <glm/glm.hpp>
#include "TwoD/Serialization/Serialization.hpp"


TD_STRUCT(
	(TD_NAME(Rect), TD_NAMESPACE(TwoD), TD_TEMPLATE((typename, T))),
	(
		TD_STRUCT_FIELD(glm::vec<2 TD_COMMA T TD_COMMA glm::defaultp>, min, TD_INIT({ 0.0f, 0.0f })),
		TD_STRUCT_FIELD(glm::vec<2 TD_COMMA T TD_COMMA glm::defaultp>, max, TD_INIT({ 0.0f, 0.0f }))
	),
	public:
		template<typename A>
		Rect(const Rect<A>& other) : min(other.min), max(other.max) {}
		Rect(glm::vec<2, T, glm::defaultp> min, glm::vec<2, T, glm::defaultp> max) : min(min), max(max) {}
		Rect() {}

		glm::vec<2, T, glm::defaultp> GetSize() const
		{
			return max - min;
		}
		T GetWidth() const
		{
			return max.x - min.x;
		}
		T GetHeight() const
		{
			return max.y - min.y;
		}
		bool IsInside(glm::vec<2, T, glm::defaultp> pos) const
		{
			return pos.x >= min.x && pos.x <= max.x && pos.y >= min.y && pos.y <= max.y;
		}
		bool operator==(const Rect<T>& other) const noexcept
		{
			return this == &other || (min == other.min && max == other.max);
		}
)