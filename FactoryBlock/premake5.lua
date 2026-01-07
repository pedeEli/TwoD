project "FactoryBlock"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.hpp",
		"src/**.cpp",
		"src/**.h"
	}

	includedirs {
		"%{wks.location}/TwoD/vendor/spdlog/include",
		"%{wks.location}/TwoD/src",
		"%{IncludeDir.SDL3}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.yaml_cpp}"
	}

	links {
		"TwoD"
	}

	postbuildcommands {
		("{COPYFILE} %[%{Dynamic.SDL3}] %[%{wks.location}/bin/" .. outputdir .. "/%{prj.name}]"),
		("{COPYFILE} %[%{Dynamic.SDL3_image}] %[%{wks.location}/bin/" .. outputdir .. "/%{prj.name}]"),
		("{COPYFILE} %[%{Dynamic.SDL3_shadercross}] %[%{wks.location}/bin/" .. outputdir .. "/%{prj.name}]"),
		("{COPYFILE} %[%{Dynamic.SDL3_ttf}] %[%{wks.location}/bin/" .. outputdir .. "/%{prj.name}]")
	}

	buildoptions { "/utf-8" }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines {
			"TD_DEBUG",
			'BASE_PATH="%{prj.location}"'
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TD_DIST"
		runtime "Release"
		optimize "on"