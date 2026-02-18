project "TwoDLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tdpch.hpp"
	pchsource "%{wks.location}/TwoD/src/tdpch.cpp"

	files {
		"%{wks.location}/TwoD/src/tdpch.cpp",
		"src/**.cpp",
		"src/**.hpp",
		"src/**.inl",
		"src/**.h",
	}

	includedirs {
		"src",
		"%{wks.location}/TwoD/src",
		"%{wks.location}/TwoD/vendor/spdlog/include",
		"%{IncludeDir.SDL3}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}"
	}

	links {
		"yaml-cpp",
		"msdf-atlas-gen"
	}

	defines {
		"YAML_CPP_STATIC_DEFINE",
		"MSDFGEN_PUBLIC="
	}

	buildoptions { "/utf-8" }
	fatalwarnings { "all" }

	filter "system:windows"
		systemversion "latest"

		defines {}

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
