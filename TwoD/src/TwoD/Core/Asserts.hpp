#pragma once
#include "TwoD/Core/Base.hpp"
#include "TwoD/Core/Log.hpp"
#include <filesystem>

#if defined(TD_DEBUG) || defined(TD_RELEASE)
	#define TD_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if (!(check)) { TD##type##ERROR(msg, __VA_ARGS__); TD_DEBUGBREAK(); } }
	#define TD_INTERNAL_ASSERT_WITH_MSG(type, check, msg) TD_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {}", msg)
	#define TD_INTERNAL_ASSERT_NO_MSG(type, check) TD_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{}' failed at {}:{}", TD_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define TD_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define TD_INTERNAL_ASSERT_GET_MACRO(...) TD_EXPAND_MACRO(TD_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, TD_INTERNAL_ASSERT_WITH_MSG, TD_INTERNAL_ASSERT_NO_MSG))

	#define TD_ASSERT(...) TD_EXPAND_MACRO(TD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
	#define TD_CORE_ASSERT(...) TD_EXPAND_MACRO(TD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
	#define TD_ASSERT(...)
	#define TD_CORE_ASSERT(...)
#endif