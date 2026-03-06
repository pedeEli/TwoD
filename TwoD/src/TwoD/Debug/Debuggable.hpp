#pragma once
#include "TwoD/Core/Base.hpp"
#ifdef TD_CREATE_DEBUGGER

#include "DebugDefines.hpp"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <glm/glm.hpp>

#include "TwoD/Assets/AssetDefines.hpp"

#define TDI_DEBUGGABLE_SINGLE(type, imgui_type)  \
	template<> \
	struct Debuggable<type> { \
		static bool Draw(type& value, const char* name) { \
			return ImGui::DragScalar(name, ImGuiDataType_ ## imgui_type, &value); \
		} \
	};

#define TDI_DEBUGGABLE_GLM(type, imgui_type) \
	template<glm::length_t N> \
	struct Debuggable<glm::vec<N, type, glm::defaultp>> { \
		static bool Draw(glm::vec<N, type, glm::defaultp>& value, const char* name) { \
			return ImGui::DragScalarN(name, ImGuiDataType_ ## imgui_type, &value, N); \
		} \
	};
#define TDI_DEBUGGABLE(type, imgui_type) \
	TDI_DEBUGGABLE_SINGLE(type, imgui_type) \
	TDI_DEBUGGABLE_GLM(type, imgui_type)

namespace TwoD
{
	TDI_DEBUGGABLE(float, Float)
	TDI_DEBUGGABLE(double, Double)
	TDI_DEBUGGABLE(int8_t, S8)
	TDI_DEBUGGABLE(int16_t, S16)
	TDI_DEBUGGABLE(int32_t, S32)
	TDI_DEBUGGABLE(int64_t, S64)
	TDI_DEBUGGABLE(uint8_t, U8)
	TDI_DEBUGGABLE(uint16_t, U16)
	TDI_DEBUGGABLE(uint32_t, U32)
	TDI_DEBUGGABLE(uint64_t, U64)
	TDI_DEBUGGABLE(bool, Bool)

	template<>
	struct Debuggable<std::string>
	{
		static bool Draw(std::string& value, const char* name)
		{
			return ImGui::InputText(name, &value);
		}
	};

	template<typename T>
	struct Debuggable<std::vector<T>>
	{
		static bool Draw(std::vector<T>& value, const char* name)
		{
			bool changed = false;
			ImGui::Text(name);
			for (size_t i = 0; i < value.size(); i++)
			{
				changed |= Debuggable<T>::Draw(value[i], std::format("{}", i).c_str());
			}
			return changed;
		}
	};

	template<typename T>
	requires(std::is_base_of_v<Asset, T>)
	struct Debuggable<T*>
	{
		using pointer = T*;
		static bool Draw(pointer& value, const char* name)
		{
			ImGui::Text("%s %p", name, value);
			return false;
		}
	};

	template<typename T>
	struct Debuggable<std::optional<T>>
	{
		static bool Draw(std::optional<T>& value, const char* name)
		{
			bool selected = value.has_value();
			bool changed = ImGui::Checkbox(name, &selected);
			if (selected != value.has_value())
			{
				if (selected)
				{
					value.emplace();
				}
				else
				{
					value.reset();
				}
			}
			if (selected)
			{
				changed |= Debuggable<T>::Draw(*value, "");
			}
			return changed;
		}
	};
}

#endif