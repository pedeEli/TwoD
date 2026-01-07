#pragma once

#include "TwoD/Core/Base.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <memory>

namespace TwoD
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}


#define TD_CORE_TRACE(...)    ::TwoD::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TD_CORE_INFO(...)     ::TwoD::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TD_CORE_WARN(...)     ::TwoD::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TD_CORE_ERROR(...)    ::TwoD::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TD_CORE_CRITICAL(...) ::TwoD::Log::GetCoreLogger()->critical(__VA_ARGS__)


#define TD_TRACE(...)         ::TwoD::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TD_INFO(...)          ::TwoD::Log::GetClientLogger()->info(__VA_ARGS__)
#define TD_WARN(...)          ::TwoD::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TD_ERROR(...)         ::TwoD::Log::GetClientLogger()->error(__VA_ARGS__)
#define TD_CRITICAL(...)      ::TwoD::Log::GetClientLogger()->critical(__VA_ARGS__)