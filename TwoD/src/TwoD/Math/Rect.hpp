#pragma once
#include <glm/glm.hpp>

namespace TwoD
{
	template<typename T>
	class Rect
	{
	public:
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

	public:
		glm::vec<2, T, glm::defaultp> min;
		glm::vec<2, T, glm::defaultp> max;
	};
}

