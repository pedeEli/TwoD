#pragma once

#include "TwoD/Core/PlatformDetection.hpp"

#ifdef TD_DEBUG
	#if defined(TD_PLATFORM_WINDOWS)
		#define TD_DEBUGBREAK() __debugbreak()
	#elif defined(TD_PLATFORM_LINUX)
		#include <signal.h>
		#define TD_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "PLatform does not support debugbreak yet!"
	#endif
#else
	#define TD_DEBUGBREAL()
#endif

#define TD_EXPAND_MACRO(x) x
#define TD_STRINGIFY_MACRO(x) #x

#include "ApplyEachMacro.hpp"

#define TD_INTERNAL_GET_MACRO_2(_1, _2, macro, ...) macro
#define TD_INTERNAL_GET_MACRO_3(_1, _2, _3, macro, ...) macro
#define TD_INTERNAL_CHOOSE_MACRO_UNWRAP(get_macro, macro_1, macro_2, ...) TD_EXPAND_MACRO(get_macro (__VA_ARGS__, macro_2, macro_1)(__VA_ARGS__))
#define TD_INTERNAL_CHOOSE_MACRO_UNWRAP_VALUES(...) __VA_ARGS__
#define TD_CHOOSE_MACRO(get_macro, macro_1, macro_2, values) TD_INTERNAL_CHOOSE_MACRO_UNWRAP(get_macro, macro_1, macro_2, TD_INTERNAL_CHOOSE_MACRO_UNWRAP_VALUES values) 
#define TD_CHOOSE_MACRO_2(macro_1, macro_2, ...) TD_CHOOSE_MACRO(TD_INTERNAL_GET_MACRO_2, macro_1, macro_2, __VA_ARGS__)
#define TD_CHOOSE_MACRO_3(macro_1, macro_2, ...) TD_CHOOSE_MACRO(TD_INTERNAL_GET_MACRO_3, macro_1, macro_2, __VA_ARGS__)


#define TD_INTERNAL_FIELDS_DEF(values) TD_CHOOSE_MACRO_3(TD_INTERNAL_FIELDS_DEF_NO_DEFAULT, TD_INTERNAL_FIELDS_DEF_WITH_DEFAULT, values)
#define TD_INTERNAL_FIELDS_DEF_WITH_DEFAULT(type, name, value) type name = value;
#define TD_INTERNAL_FIELDS_DEF_NO_DEFAULT(type, name) type name;

#define TD_INTERNAL_FIELDS_LOAD(values) TD_INTERNAL_FIELDS_LOAD_UNWRAP values
#define TD_INTERNAL_FIELDS_LOAD_UNWRAP(type, name, ...) \
	if (node[#name]) { name = node[#name].as<type>(); }

#define TD_INTERNAL_FIELDS(params, ...) TD_APPLY_EACH(TD_INTERNAL_FIELDS_DEF, __VA_ARGS__) \
	void Load params override { \
		TD_APPLY_EACH(TD_INTERNAL_FIELDS_LOAD, __VA_ARGS__) \
	}
#define TD_FIELD(...) (__VA_ARGS__)

#define TD_COMPONENT_FIELDS(...) TD_INTERNAL_FIELDS((const YAML::Node& node), __VA_ARGS__)
#define TD_ASSET_FIELDS(...) TD_INTERNAL_FIELDS((const YAML::Node& node), __VA_ARGS__)

#include "TwoD/Core/Asserts.hpp"
#include "TwoD/Core/Log.hpp"