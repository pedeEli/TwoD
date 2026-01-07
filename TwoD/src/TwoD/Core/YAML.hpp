#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::fvec3>
	{
		static Node encode(const glm::fvec3& rhs);
		static bool decode(const Node& node, glm::fvec3& rhs);
	};

	template<>
	struct convert<glm::fvec2>
	{
		static Node encode(const glm::fvec2& rhs);
		static bool decode(const Node& node, glm::fvec2& rhs);
	};

	template<typename T>
	struct convert<std::optional<T>>
	{
		static Node encode(const std::optional<T>& rhs)
		{
			TD_CORE_ASSERT(false, "cannot convert std::optional to yaml node")
		}
		static bool decode(const Node& node, std::optional<T>& rhs)
		{
			try
			{
				rhs = node.as<T>();
			}
			catch (YAML::ParserException&) {}
			return true;
		}
	};
}