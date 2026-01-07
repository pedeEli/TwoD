

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