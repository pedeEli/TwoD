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

#ifndef TD_DIST
	#define TD_CREATE_DEBUGGER
#endif

#include "TwoD/Core/Asserts.hpp"
#include "TwoD/Core/Log.hpp"