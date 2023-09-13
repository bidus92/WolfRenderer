workspace "WolfRenderer"

	architecture "x64"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Dear_ImGui"] = "%{wks.location}/WolfRenderer/vendor/Dear_ImGui"
IncludeDir["SDL3"] = "%{wks.location}/WolfRenderer/vendor/SDL3/include"


include "WolfRenderer/vendor/Dear_ImGui/imgui"

include "WolfRenderer/vendor/SDL3"

	
project "WolfRenderer"

	location "%{prj.name}/WolfRenderer"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{wks.location}/WolfRenderer/WolfRenderer/src/pch.cpp"
		
	files
	{
		"%{wks.location}/WolfRenderer/WolfRenderer/src/**.h",
		"%{wks.location}/WolfRenderer/WolfRenderer/src/**.cpp"
	}

	includedirs
	{	
		"%{prj.name}/WolfRenderer/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.Dear_ImGui}",
		"%{IncludeDir.SDL3}",
		"%{IncludeDir.SDL3}/build_config",
		"%{IncludeDir.SDL3}/SDL3"
	}

	libdirs
	{
		"%{wks.location}/WolfRenderer/vendor/bin/" .. outputdir .. "/SDL3",
		"%{wks.location}/WolfRenderer/vendor/bin/" .. outputdir .. "Dear_ImGui"
	}

	links
	{
		"Dear_ImGui",
		"SDL3"		
	}



	filter "system:windows"
		staticruntime "Off"
		systemversion "latest"


defines
{
	"WLFR_PLATFORM_WINDOWS",
	"WLFR_BUILD_DLL"
}

postbuildcommands
{
	("{COPY} %{cfg.buildtarget.relpath} ../../bin/" .. outputdir .. "/Sandbox")
}

filter "configurations:Debug"
	defines "WLFR_DEBUG"
	symbols "On"
	buildoptions "/MTd"

filter "configurations:Release"
	defines "WLFR_RELEASE"
	optimize "On"
	buildoptions "/MT"

filter "configurations:Dist"
	defines "WLFR_DIST"
	optimize "On"




project "Sandbox"

location "Sandbox"

kind "ConsoleApp"

language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/%{wks.name}/WolfRenderer/src",
		"%{wks.location}/%{wks.name}/vendor/spdlog/include",
		"%{IncludeDir.Dear_ImGui}",
		"%{IncludeDir.SDL3}",
		"%{prj.name}/src"
	}

	links
	{
		"WolfRenderer"
	}

filter "system:windows"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"


	defines
	{
		"WLFR_PLATFORM_WINDOWS"
	}




filter "configurations:Debug"
	defines "WLFR_DEBUG"
	symbols "On"
	buildoptions "/MDd"

filter "configurations:Release"
	defines "WLFR_RELEASE"
	optimize "On"
	buildoptions "/MD"

filter "configurations:Dist"
	defines "WLFR_DIST"
	optimize "On"