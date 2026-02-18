project "yaml-cpp"
	kind "StaticLib"
	language "C++"

	location "yaml-cpp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"yaml-cpp/src/**.h",
		"yaml-cpp/src/**.hpp",
		"yaml-cpp/src/**.cpp",
	}

	includedirs {
		"yaml-cpp/include"
	}

	defines {
		"yaml_cpp_EXPORTS"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter {"configurations:Release", "configurations:Dist"}
		runtime "Release"
		optimize "on"

project "imgui"
	kind "StaticLib"
	language "C++"

	location "imgui"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"imgui/imgui.h",
		"imgui/imgui.cpp",
		"imgui/imgui_tables.cpp",
		"imgui/imgui_widgets.cpp",
		"imgui/imgui_demo.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_internal.h",
		"imgui/backends/imgui_impl_sdl3.h",
		"imgui/backends/imgui_impl_sdl3.cpp",
		"imgui/backends/imgui_impl_sdlgpu3.h",
		"imgui/backends/imgui_impl_sdlgpu3.cpp",
		"imgui/backends/imgui_impl_sdlgpu3_shaders.h",
		"imgui/misc/cpp/imgui_stdlib.cpp",
		"imgui/misc/cpp/imgui_stdlib.h"
	}

	includedirs {
		"imgui",
		"%{IncludeDir.SDL3}"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter {"configurations:Release", "configurations:Dist"}
		runtime "Release"
		optimize "on"