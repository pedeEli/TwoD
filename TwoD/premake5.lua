project "TwoD"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tdpch.hpp"
	pchsource "src/tdpch.cpp"

	files {
		"src/**.cpp",
		"src/**.hpp",
		"src/**.inl",
		"src/**.h",

		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

	includedirs {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.SDL3}",
		"%{IncludeDir.SDL3_image}",
		"%{IncludeDir.SDL3_shadercross}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.glm}"
	}

	links {
		"yaml-cpp",
		"msdf-atlas-gen",
		"%{Library.SDL3}",
		"%{Library.SDL3_image}",
		"%{Library.SDL3_shadercross}"
	}

	defines {
		"YAML_CPP_STATIC_DEFINE",
		"MSDFGEN_PUBLIC="
	}

	buildoptions { "/utf-8" }

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
