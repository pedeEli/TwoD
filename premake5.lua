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
	usestandardpreprocessor 'on'

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "TwoD/vendor"
	include "TwoDLib/vendor"
group ""

group "Core"
	include "TwoD"
	include "TwoDLib"
group ""

group "Misc"
	include "FactoryBlock"
	include "Kniffel"
group ""