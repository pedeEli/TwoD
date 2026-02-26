#pragma once
#include "TwoD/Core/Base.hpp"
#ifdef TD_IMGUI

#include "DebugDefines.hpp"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <glm/glm.hpp>

#include "TwoD/Assets/AssetDefines.hpp"

namespace TwoD
{
	template<>
	struct ImGuiType<float>
	{
		static bool Draw(float& value, const char* name)
		{
			return ImGui::DragFloat(name, &value);
		}
	};

	template<>
	struct ImGuiType<bool>
	{
		static bool Draw(bool& value, const char* name)
		{
			return ImGui::Checkbox(name, &value);
		}
	};

	template<>
	struct ImGuiType<glm::fvec2>
	{
		static bool Draw(glm::fvec2& value, const char* name)
		{
			return ImGui::DragFloat2(name, (float*)&value);
		}
	};

	template<>
	struct ImGuiType<glm::fvec3>
	{
		static bool Draw(glm::fvec3& value, const char* name)
		{
			return ImGui::DragFloat3(name, (float*)&value);
		}
	};

	template<>
	struct ImGuiType<glm::fvec4>
	{
		static bool Draw(glm::fvec4& value, const char* name)
		{
			return ImGui::DragFloat4(name, (float*)&value);
		}
	};

	template<>
	struct ImGuiType<glm::u8vec4>
	{
		static bool Draw(glm::u8vec4& value, const char* name)
		{
			return ImGui::DragInt4(name, (int*)&value);
		}
	};

	template<>
	struct ImGuiType<std::string>
	{
		static bool Draw(std::string& value, const char* name)
		{
			return ImGui::InputText(name, &value);
		}
	};

	template<>
	struct ImGuiType<std::string_view>
	{
		static bool Draw(std::string_view& value, const char* name)
		{
			std::string str(value);
			auto changed = ImGui::InputText(name, &str);
			if (changed)
			{
				value = str;
			}
			return changed;
		}
	};

	template<typename T>
	requires (requires(T& value, const char* name)
	{
		{ ImGuiType<T>::Draw(value, name) } -> std::same_as<bool>;
	})
	struct ImGuiType<std::vector<T>>
	{
		static bool Draw(std::vector<T>& value, const char* name)
		{
			bool changed = false;
			ImGui::Text(name);
			for (size_t i = 0; i < value.size(); i++)
			{
				changed |= ImGuiType<T>::Draw(value[i], std::format("{}", i).c_str());
			}
			return changed;
		}
	};

	template<typename T>
	requires(std::is_integral_v<T>)
	struct ImGuiType<T>
	{
		static bool Draw(T& value, const char* name)
		{
			return ImGui::DragInt(name, (int*)&value);
		}
	};

	template<typename T>
	requires(std::is_base_of_v<Asset, T>)
	struct ImGuiType<T*>
	{
		using pointer = T*;
		static bool Draw(pointer& value, const char* name)
		{
			ImGui::Text("%s %p", name, value);
			return false;
		}
	};

	template<typename T>
	struct ImGuiType<std::optional<T>>
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
				changed |= ImGuiType<T>::Draw(*value, "");
			}
			return changed;
		}
	};
}

#endif