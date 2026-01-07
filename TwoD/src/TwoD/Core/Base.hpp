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

#define TD_INTERNAL_APPLY_EACH_1(F, X1) F(X1)
#define TD_INTERNAL_APPLY_EACH_2(F, X1, X2) F(X1) F(X2)
#define TD_INTERNAL_APPLY_EACH_3(F, X1, X2, X3) F(X1) F(X2) F(X3)
#define TD_INTERNAL_APPLY_EACH_4(F, X1, X2, X3, X4) F(X1) F(X2) F(X3) F(X4)
#define TD_INTERNAL_APPLY_EACH_5(F, X1, X2, X3, X4, X5) F(X1) F(X2) F(X3) F(X4) F(X5)
#define TD_INTERNAL_APPLY_EACH_6(F, X1, X2, X3, X4, X5, X6) F(X1) F(X2) F(X3) F(X4) F(X5) F(X6)
#define TD_INTERNAL_APPLY_EACH_7(F, X1, X2, X3, X4, X5, X6, X7) F(X1) F(X2) F(X3) F(X4) F(X5) F(X6) F(X7)
#define TD_INTERNAL_APPLY_EACH_8(F, X1, X2, X3, X4, X5, X6, X7, X8) F(X1) F(X2) F(X3) F(X4) F(X5) F(X6) F(X7) F(X8) 
#define TD_INTERNAL_APPLY_EACH_9(F, X1, X2, X3, X4, X5, X6, X7, X8, X9) F(X1) F(X2) F(X3) F(X4) F(X5) F(X6) F(X7) F(X8) F(X9)
#define TD_INTERNAL_APPLY_EACH_10(F, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10) F(X1) F(X2) F(X3) F(X4) F(X5) F(X6) F(X7) F(X8) F(X9) F(X10)
#define TD_INTERNAL_GET_EACH_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, MACRO, ...) MACRO
#define TD_APPLY_EACH(F, ...) TD_EXPAND_MACRO( \
	TD_INTERNAL_GET_EACH_MACRO( \
		__VA_ARGS__, \
		TD_INTERNAL_APPLY_EACH_10, TD_INTERNAL_APPLY_EACH_9, \
		TD_INTERNAL_APPLY_EACH_8, TD_INTERNAL_APPLY_EACH_7, \
		TD_INTERNAL_APPLY_EACH_6, TD_INTERNAL_APPLY_EACH_5, \
		TD_INTERNAL_APPLY_EACH_4, TD_INTERNAL_APPLY_EACH_3, \
		TD_INTERNAL_APPLY_EACH_2, TD_INTERNAL_APPLY_EACH_1, \
	) \
	(F, __VA_ARGS__) \
)

#define TD_INTERNAL_FIELDS_DEF(values) TD_INTERNAL_FIELDS_DEF_UNWRAP values
#define TD_INTERNAL_FIELDS_DEF_WITH_DEFAULT(type, name, value) type name = value;
#define TD_INTERNAL_FIELDS_DEF_NO_DEFAULT(type, name) type name;
#define TD_INTERNAL_FIELDS_DEF_GET_MACRO(_1, _2, _3, macro, ...) macro
#define TD_INTERNAL_FIELDS_DEF_UNWRAP(...) TD_EXPAND_MACRO(TD_INTERNAL_FIELDS_DEF_GET_MACRO( \
	__VA_ARGS__, \
	TD_INTERNAL_FIELDS_DEF_WITH_DEFAULT, \
	TD_INTERNAL_FIELDS_DEF_NO_DEFAULT \
)(__VA_ARGS__))

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