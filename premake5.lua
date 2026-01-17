include "Dependencies.lua"

workspace "TwoD"
	architecture "x86_64"
	startproject "FactoryBlock"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "TwoD/vendor"
group ""

group "Core"
	include "TwoD"
	include "TwoDLib"
group ""

group "Misc"
	include "FactoryBlock"
group ""