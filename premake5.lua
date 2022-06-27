-- premake5.lua
workspace "CalicoMipsSimulator"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Calico Mips Simulator"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["ImGui"] = "CalicoMipsSimulator/vendor/imgui"
include "CalicoMipsSimulator/vendor/imgui"
project "CalicoMipsSimulator"
	location "CalicoMipsSimulator"
	kind "WindowedApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/MIPS",
		"%{prj.name}/src/Editor",
		"%{prj.name}/src/AssemblyCore",
		"%{prj.name}/src/Noble-16",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/fast-cpp-csv-parser",
		"%{IncludeDir.ImGui}"
		
	}

	links 
	{ 
      "d3d9.lib",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CL_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "CL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CL_DIST"
		runtime "Release"
		optimize "On"