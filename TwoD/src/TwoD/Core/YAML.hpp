#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "TwoD/Core/Base.hpp"
#include "TwoD/Debug/ImGui.hpp"

#define TD_INTERNAL_YAML_STRUCT_FIELD(values) TD_CHOOSE_MACRO_4(TD_INTERNAL_YAML_STRUCT_FIELD_NO_DEFAULT, TD_INTERNAL_YAML_STRUCT_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_YAML_STRUCT_FIELD_NO_DEFAULT(preprocess, type, name) type name;
#define TD_INTERNAL_YAML_STRUCT_FIELD_WITH_DEFAULT(preprocess, type, name, value) type name = value;

#define TD_INTERNAL_YAML_STRUCT_FIELD_NO_PARSING(...)
#define TD_INTERNAL_YAML_STRUCT_ENCODE(values) TD_INTERNAL_YAML_STRUCT_ENCODE_UNWRAP values
#define TD_INTERNAL_YAML_STRUCT_ENCODE_UNWRAP(preprocess, type, name, ...) preprocess(node[#name] = rhs.name;)

#define TD_INTERNAL_YAML_STRUCT_DECODE(values) TD_CHOOSE_MACRO_4(TD_INTERNAL_YAML_STRUCT_DECODE_NO_DEFAULT, TD_INTERNAL_YAML_STRUCT_DECODE_WITH_DEFAULT, values)
#define TD_INTERNAL_YAML_STRUCT_DECODE_NO_DEFAULT(preprocess, type, vname) preprocess(if (!node[#vname]) { TD_CORE_ERROR("missing field {} in {}", #vname, typeid(decltype(rhs)).name()); return false; } \
	rhs.vname = node[#vname].as<type>();)
#define TD_INTERNAL_YAML_STRUCT_DECODE_WITH_DEFAULT(preprocess, type, name, ...) preprocess(if (node[#name]) { rhs.name = node[#name].as<type>(); })

#define TD_INTERNAL_YAML_STRUCT_YAML(ns_name, struct_name, ...) namespace YAML { \
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

#define TD_INTERNAL_YAML_STRUCT_IMGUI_FIELD(values) TD_INTERNAL_YAML_STRUCT_IMGUI_FIELD_UNWRAP values
#define TD_INTERNAL_YAML_STRUCT_IMGUI_FIELD_UNWRAP(preprocess, type, name, ...) preprocess(changed |= ImGuiType<type>::Draw(value.name, #name);)

#define TD_INTERNAL_YAML_STRUCT_IMGUI(ns_name, struct_name, ...)  namespace TwoD { \
		template<> \
		struct ImGuiType<::ns_name::struct_name> { \
			static bool Draw(::ns_name::struct_name& value, const char* name) { \
				ImGui::Text(name); \
				bool changed = false; \
				TD_APPLY_EACH(TD_INTERNAL_YAML_STRUCT_IMGUI_FIELD, __VA_ARGS__) \
				return changed; \
			} \
		}; \
	}

#define TD_YAML_STRUCT(ns_name, struct_name, ...) namespace ns_name { \
		struct struct_name { \
			TD_APPLY_EACH(TD_INTERNAL_YAML_STRUCT_FIELD, __VA_ARGS__) \
		}; \
	} \
	TD_INTERNAL_YAML_STRUCT_YAML(ns_name, struct_name, __VA_ARGS__) \
	TD_INTERNAL_YAML_STRUCT_IMGUI(ns_name, struct_name, __VA_ARGS__)

#define TD_YAML_STRUCT_FIELD(...) (TD_EXPAND_MACRO, __VA_ARGS__)
#define TD_YAML_STRUCT_FIELD_NO_PARSING(...) (TD_INTERNAL_YAML_STRUCT_FIELD_NO_PARSING, __VA_ARGS__)


#define TD_INTERNAL_YAML_ENUM_FIELD(values) TD_CHOOSE_MACRO_2(TD_INTERNAL_YAML_ENUM_FIELD_NO_DEFAULT, TD_INTERNAL_YAML_ENUM_FIELD_WITH_DEFAULT, values)
#define TD_INTERNAL_YAML_ENUM_FIELD_NO_DEFAULT(name) name,
#define TD_INTERNAL_YAML_ENUM_FIELD_WITH_DEFAULT(name, value) name = value,

#define TD_INTERNAL_YAML_ENUM_ENCODE(values) TD_INTERNAL_YAML_ENUM_ENCODE_UNWRAP values
#define TD_INTERNAL_YAML_ENUM_ENCODE_UNWRAP(name, ...) case internal_enum::name: return Node(#name);
#define TD_INTERNAL_YAML_ENUM_DECODE(values) TD_INTERNAL_YAML_ENUM_DECODE_UNWRAP values
#define TD_INTERNAL_YAML_ENUM_DECODE_UNWRAP(name, ...) if (str == #name) { rhs = internal_enum::name; return true; }

#define TD_INTERNAL_YAML_ENUM_BIT_OPERATOR(enum_name, op) constexpr enum_name operator op(enum_name a, enum_name b) noexcept { \
	return static_cast<enum_name>(static_cast<std::underlying_type_t<enum_name>>(a) op static_cast<std::underlying_type_t<enum_name>>(b)); \
} \
constexpr enum_name& operator op##=(enum_name& a, enum_name b) noexcept { \
	a = a op b; \
	return a; \
}
#define TD_INTERNAL_YAML_ENUM_BOOL_OPERATOR(enum_name, op) constexpr bool operator op(enum_name a, enum_name b) noexcept { \
	return static_cast<std::underlying_type_t<enum_name>>(a) op static_cast<std::underlying_type_t<enum_name>>(b); \
}
#define TD_INTERNAL_YAML_ENUM_BIT_UNARY_OPERATOR(enum_name, op) constexpr enum_name operator op(enum_name a) noexcept { \
	return static_cast<enum_name>(op static_cast<std::underlying_type_t<enum_name>>(a)); \
}

#define TD_INTERNAL_YAML_ENUM_YAML_ENCODE(ns_name, enum_name, ...) static Node encode(const ns_name::enum_name& rhs) { \
		using internal_enum = ns_name::enum_name; \
		switch (rhs) { \
			TD_APPLY_EACH(TD_INTERNAL_YAML_ENUM_ENCODE, __VA_ARGS__) \
		} \
	}
#define TD_INTERNAL_YAML_ENUM_YAML_DECODE(ns_name, enum_name, ...) static bool decode(const Node& node, ns_name::enum_name& rhs) { \
		using internal_enum = ns_name::enum_name; \
		auto f = [](const std::string& str, ns_name::enum_name& rhs) { \
			TD_APPLY_EACH(TD_INTERNAL_YAML_ENUM_DECODE, __VA_ARGS__) \
			return false; \
		}; \
		if (node.IsSequence()) { \
			for (auto n : node) { \
				ns_name::enum_name temp; \
				auto str = n.as<std::string>(); \
				if (!f(str, temp)) { \
					TD_CORE_ERROR("{} does not match any values of {}", str, typeid(decltype(rhs)).name()); \
					return false; \
				} \
				rhs |= temp; \
			} \
			return true; \
		} \
		auto str = node.as<std::string>(); \
		if (f(str, rhs)) { return true; } \
		TD_CORE_ERROR("{} does not match any values of {}", str, typeid(decltype(rhs)).name()); \
		return false; \
	}

#define TD_INTERNAL_YAML_ENUM_YAML(ns_name, enum_name, ...) namespace YAML { \
	template<> \
	struct convert<ns_name::enum_name>{ \
		TD_INTERNAL_YAML_ENUM_YAML_ENCODE(ns_name, enum_name, __VA_ARGS__) \
		TD_INTERNAL_YAML_ENUM_YAML_DECODE(ns_name, enum_name, __VA_ARGS__) \
	}; \
}

#define TD_INTERNAL_YAML_ENUM_IMGUI_FIELD(values) TD_INTERNAL_YAML_ENUM_IMGUI_FIELD_UNWRAP values
#define TD_INTERNAL_YAML_ENUM_IMGUI_FIELD_UNWRAP(name, ...) { \
		bool selected = static_cast<std::underlying_type_t<internal_enum>>(value & internal_enum::name) != 0; \
		changed |= ImGui::Selectable(#name, &selected); \
		if (selected) { \
			value |= internal_enum::name; \
		} else { \
			value &= ~internal_enum::name; \
		} \
	}

#define TD_INTERNAL_YAML_ENUM_IMGUI(ns_name, enum_name, ...) namespace TwoD { \
		template<> \
		struct ImGuiType<::ns_name::enum_name> { \
			static bool Draw(::ns_name::enum_name& value, const char* name) { \
				using internal_enum = ns_name::enum_name; \
				ImGui::Text(name); \
				bool changed = false; \
				TD_APPLY_EACH(TD_INTERNAL_YAML_ENUM_IMGUI_FIELD, __VA_ARGS__) \
				return changed; \
			} \
		}; \
	}

#define TD_INTERNAL_YAML_ENUM(ns_name, enum_name, enum_name_with_base, ...) namespace ns_name { \
	enum class enum_name_with_base { \
			TD_APPLY_EACH(TD_INTERNAL_YAML_ENUM_FIELD, __VA_ARGS__) \
		}; \
		TD_INTERNAL_YAML_ENUM_BIT_OPERATOR(enum_name, |) \
		TD_INTERNAL_YAML_ENUM_BIT_OPERATOR(enum_name, &) \
		TD_INTERNAL_YAML_ENUM_BIT_OPERATOR(enum_name, ^) \
		TD_INTERNAL_YAML_ENUM_BOOL_OPERATOR(enum_name, ==) \
		TD_INTERNAL_YAML_ENUM_BOOL_OPERATOR(enum_name, !=) \
		TD_INTERNAL_YAML_ENUM_BIT_UNARY_OPERATOR(enum_name, ~) \
	} \
	TD_INTERNAL_YAML_ENUM_YAML(ns_name, enum_name, __VA_ARGS__) \
	TD_INTERNAL_YAML_ENUM_IMGUI(ns_name, enum_name, __VA_ARGS__)

#define TD_YAML_ENUM_WITH_BASE(ns_name, enum_name, base, ...) TD_INTERNAL_YAML_ENUM(ns_name, enum_name, enum_name : base, __VA_ARGS__)
#define TD_YAML_ENUM(ns_name, enum_name, ...) TD_INTERNAL_YAML_ENUM(ns_name, enum_name, enum_name, __VA_ARGS__)
#define TD_YAML_ENUM_FIELD(...) (__VA_ARGS__)


//#include <imgui.h>
//#include "TwoD/Debug/ImGui.hpp"
//TD_YAML_STRUCT(Foo, Bar, TD_YAML_STRUCT_FIELD(uint32_t, age))
//TD_YAML_ENUM(Foo, Bar, TD_YAML_ENUM_FIELD(ONE))

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