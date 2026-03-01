#pragma once
#include <glm/glm.hpp>
#include "TwoD/Core/YAML.hpp"

namespace TwoD
{
	template<typename T>
	class Rect
	{
	public:
		Rect() {};
		Rect(glm::vec<2, T, glm::defaultp> min, glm::vec<2, T, glm::defaultp> max) : min(min), max(max) {}
		
		~Rect() = default;
		Rect(const Rect& other) noexcept = default;
		Rect(Rect&& other) noexcept = default;
		Rect& operator=(const Rect& other) noexcept = default;
		Rect& operator=(Rect&& other) noexcept = default;

		template<typename A>
		Rect(const Rect<A>& other) : min(other.min), max(other.max) {}

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

	public:
		glm::vec<2, T, glm::defaultp> min;
		glm::vec<2, T, glm::defaultp> max;
	};
}

namespace YAML
{
	template<typename T>
	struct convert<::TwoD::Rect<T>>
	{
		static Node encode(const ::TwoD::Rect<T>& rhs)
		{
			Node node;
			node["min"] = rhs.min;
			node["max"] = rhs.max;
		}
		static bool decode(const Node& node, ::TwoD::Rect<T>& rhs)
		{
			if (node["min"])
			{
				rhs.min = node["min"].as<decltype(rhs.min)>();
			}
			if (node["max"])
			{
				rhs.max = node["max"].as<decltype(rhs.max)>();
			}
			return true;
		}
	};
}

