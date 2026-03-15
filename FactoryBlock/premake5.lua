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
		"%{wks.location}/TwoD/src",
		"%{wks.location}/TwoDLib/src"
	}

	externalincludedirs {
		"%{wks.location}/TwoD/vendor/spdlog/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}"
	}

	links {
		"TwoD",
		"TwoDLib"
	}

	postbuildcommands {
		("{COPYFILE} %[%{Dynamic.SDL3}] %[%{wks.location}/bin/" .. outputdir .. "/%{prj.name}]"),
		("{COPYFILE} %[%{Dynamic.SDL3_image}] %[%{wks.location}/bin/" .. outputdir .. "/%{prj.name}]"),
		("{COPYFILE} %[%{Dynamic.SDL3_shadercross}] %[%{wks.location}/bin/" .. outputdir .. "/%{prj.name}]")
	}

	buildoptions { "/utf-8" }
	warnings "Extra"
	fatalwarnings { "all" }

	filter "action:vs*"
        buildoptions { "/we4834", "/w15240" }
	
	defines {
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "TD_DEBUG"
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