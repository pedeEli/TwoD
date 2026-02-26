#include "tdpch.hpp"
#include "YAML.hpp"

namespace YAML
{
	Node convert<glm::fvec3>::encode(const glm::fvec3& rhs)
	{
		Node node;
		node["x"] = rhs.x;
		node["y"] = rhs.y;
		node["z"] = rhs.z;
		return node;
	}
	bool convert<glm::fvec3>::decode(const Node& node, glm::fvec3& rhs)
	{
		auto x = node["x"];
		auto y = node["y"];
		auto z = node["z"];
		if (x)
		{
			rhs.x = x.as<float>();
		}
		if (y)
		{
			rhs.y = y.as<float>();
		}
		if (z)
		{
			rhs.z = z.as<float>();
		}
		return true;
	}

	Node convert<glm::fvec2>::encode(const glm::fvec2& rhs)
	{
		Node node;
		node["x"] = rhs.x;
		node["y"] = rhs.y;
		return node;
	}
	bool convert<glm::fvec2>::decode(const Node& node, glm::fvec2& rhs)
	{
		auto x = node["x"];
		auto y = node["y"];
		if (x)
		{
			rhs.x = x.as<float>();
		}
		if (y)
		{
			rhs.y = y.as<float>();
		}
		return true;
	}

	Node convert<glm::u8vec4>::encode(const glm::u8vec4& rhs)
	{
		Node node;
		node["x"] = rhs.x;
		node["y"] = rhs.y;
		node["z"] = rhs.z;
		node["w"] = rhs.w;
		return node;
	}
	bool convert<glm::u8vec4>::decode(const Node& node, glm::u8vec4& rhs)
	{
		auto x = node["x"];
		auto y = node["y"];
		auto z = node["z"];
		auto w = node["w"];
		if (x)
		{
			rhs.x = x.as<uint8_t>();
		}
		if (y)
		{
			rhs.y = y.as<uint8_t>();
		}
		if (z)
		{
			rhs.z = z.as<uint8_t>();
		}
		if (w)
		{
			rhs.w = w.as<uint8_t>();
		}
		return true;
	}
}