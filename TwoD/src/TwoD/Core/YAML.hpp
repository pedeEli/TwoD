#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "TwoD/Core/Base.hpp"

#define TD_INTERNAL_YAML_STRUCT_FIELD(values) TD_CHOOSE_MACRO_3(TD_INTERNAL_YAML_STRUCT_FIELD_NO_DEFAULT, TD_INTERNAL_YAML_STRUCT_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_YAML_STRUCT_FIELD_NO_DEFAULT(type, name) type name;
#define TD_INTERNAL_YAML_STRUCT_FIELD_WITH_DEFAULT(type, name, value) type name = value;

#define TD_INTERNAL_YAML_STRUCT_ENCODE(values) TD_INTERNAL_YAML_STRUCT_ENCODE_UNWRAP values
#define TD_INTERNAL_YAML_STRUCT_ENCODE_UNWRAP(type, name, ...) node[#name] = rhs.name;

#define TD_INTERNAL_YAML_STRUCT_DECODE(values) TD_CHOOSE_MACRO_3(TD_INTERNAL_YAML_STRUCT_DECODE_NO_DEFAULT, TD_INTERNAL_YAML_STRUCT_DECODE_WITH_DEFAULT, values)
#define TD_INTERNAL_YAML_STRUCT_DECODE_NO_DEFAULT(type, name) if (!node[#name]) { return false; } \
	rhs.name = node[#name].as<type>();
#define TD_INTERNAL_YAML_STRUCT_DECODE_WITH_DEFAULT(type, name, ...) if (node[#name]) { rhs.name = node[#name].as<type>(); }

#define TD_YAML_STRUCT(ns_name, struct_name, ...) namespace ns_name { \
	struct struct_name { \
		TD_APPLY_EACH(TD_INTERNAL_YAML_STRUCT_FIELD, __VA_ARGS__) \
	}; \
} \
namespace YAML { \
	template<> \
	struct convert<ns_name::struct_name> { \
		static Node encode(const ns_name::struct_name& rhs) { \
			Node node; \
			TD_APPLY_EACH(TD_INTERNAL_YAML_STRUCT_ENCODE, __VA_ARGS__) \
			return node; \
		} \
		static bool decode(const Node& node, ns_name::struct_name& rhs) { \
			TD_APPLY_EACH(TD_INTERNAL_YAML_STRUCT_DECODE, __VA_ARGS__) \
			return true; \
		} \
	}; \
}
#define TD_YAML_STRUCT_FIELD(...) (__VA_ARGS__)


#define TD_INTERNAL_YAML_ENUM_FIELD(values) TD_CHOOSE_MACRO_2(TD_INTERNAL_YAML_ENUM_FIELD_NO_DEFAULT, TD_INTERNAL_YAML_ENUM_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_YAML_ENUM_FIELD_NO_DEFAULT(name) name,
#define TD_INTERNAL_YAML_ENUM_FIELD_WITH_DEFAULT(name, value) name = value,

#define TD_INTERNAL_YAML_ENUM_ENCODE(values) TD_INTERNAL_YAML_ENUM_ENCODE_UNWRAP values
#define TD_INTERNAL_YAML_ENUM_ENCODE_UNWRAP(name, ...) case internal_enum::name: return Node(#name);
#define TD_INTERNAL_YAML_ENUM_DECODE(values) TD_INTERNAL_YAML_ENUM_DECODE_UNWRAP values
#define TD_INTERNAL_YAML_ENUM_DECODE_UNWRAP(name, ...) if (str == #name) { rhs = internal_enum::name; return true; }

#define TD_INTERNAL_YAML_ENUM(ns_name, enum_name, enum_name_with_base, ...) namespace ns_name { \
	enum class enum_name_with_base { \
		TD_APPLY_EACH(TD_INTERNAL_YAML_ENUM_FIELD, __VA_ARGS__) \
	}; \
} \
namespace YAML { \
	template<> \
	struct convert<ns_name::enum_name>{ \
		static Node encode(const ns_name::enum_name& rhs) { \
			using internal_enum = ns_name::enum_name; \
			switch (rhs) { \
				TD_APPLY_EACH(TD_INTERNAL_YAML_ENUM_ENCODE, __VA_ARGS__) \
			} \
		} \
		static bool decode(const Node& node, ns_name::enum_name& rhs) { \
			using internal_enum = ns_name::enum_name; \
			auto str = node.as<std::string>(); \
			TD_APPLY_EACH(TD_INTERNAL_YAML_ENUM_DECODE, __VA_ARGS__) \
			return false; \
		} \
	}; \
}
#define TD_YAML_ENUM_WITH_BASE(ns_name, enum_name, base, ...) TD_INTERNAL_YAML_ENUM(ns_name, enum_name, enum_name : base, __VA_ARGS__)
#define TD_YAML_ENUM(ns_name, enum_name, ...) TD_INTERNAL_YAML_ENUM(ns_name, enum_name, enum_name, __VA_ARGS__)
#define TD_YAML_ENUM_FIELD(...) (__VA_ARGS__)

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