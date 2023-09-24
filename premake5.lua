workspace "WolfRenderer"

	architecture "x64"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directory table
IncludeDir = {}

IncludeDir["Dear_ImGui"] = "%{wks.location}/WolfRenderer/WolfRenderer/vendor/Dear_ImGui"
IncludeDir["SDL3"] = "%{wks.location}/WolfRenderer/WolfRenderer/vendor/SDL3/include"
IncludeDir["spdlog"] = "%{wks.location}/WolfRenderer/WolfRenderer/vendor/spdlog/include"


--Library directory table
LibraryDir = {}

LibraryDir["SDL3"] = "%{wks.location}/WolfRenderer/WolfRenderer/vendor/SDL3/bin/" .. outputdir .. "/SDL3"
LibraryDir["Dear_ImGui"] = "%{wks.location}/WolfRenderer/WolfRenderer/vendor/Dear_ImGui/imgui/bin/" .. outputdir .. "Dear_ImGui"
LibraryDir["WolfRenderer"] = "%{wks.location}/bin/" .. outputdir .. "/WolfRenderer"

include "WolfRenderer/WolfRenderer/vendor/Dear_ImGui/imgui"

include "WolfRenderer/WolfRenderer/vendor/SDL3"

	
project "WolfRenderer"

	location "%{prj.name}/WolfRenderer"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"
	--linkoptions {"/NODEFAULTLIB"}
	

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
		"%{wks.location}/WolfRenderer/WolfRenderer/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.Dear_ImGui}",
		"%{IncludeDir.SDL3}",
		"%{IncludeDir.SDL3}/build_config",
		"%{IncludeDir.SDL3}/SDL3"
	}

	libdirs
	{
		"%{LibraryDir.SDL3}",
		"%{LibraryDir.Dear_ImGui}"
	}

	links
	{
		"Dear_ImGui",
		"SDL3-static",
		"winmm.lib",
		"version.lib",
		"imm32.lib",
		"setupapi.lib",
		--"libcmt.lib",
		--"libucrtd.lib"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/bin/" .. outputdir .. "/Sandbox")
	}



	filter "system:windows"
		staticruntime "Off"
		systemversion "latest"


defines
{
	"SDL_STATIC_LIB",
	"_WIN64",
	"WLFR_PLATFORM_WINDOWS",
	"WLFR_BUILD_DLL"
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



project "Sandbox"

    location "%{prj.name}"

    kind "ConsoleApp"

    language "C++"

	targetdir ("bin/" .. outputdir .. "/Sandbox")
	objdir ("bin-int/" .. outputdir .. "/Sandbox")

	files	
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/WolfRenderer/WolfRenderer/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.Dear_ImGui}",
		"%{IncludeDir.SDL3}",
		"%{IncludeDir.SDL3}/build_config",
		"%{IncludeDir.SDL3}/SDL3",
		"%{prj.name}/src"
	}
	
	libdirs
	{
		"%{LibraryDir.WolfRenderer}"
	}

	links
	{
		"WolfRenderer"
		--"winmm.lib",
		--"imm32.lib",
		--"version.lib"
		--"libucrtd.lib",
		--"libcmt.lib",
		--"NtosKrnl.lib"
		--,
		--"winmm.lib", 
		--"setupapi.lib", "version.lib",
		 --"Imm32.lib"
	}

filter "system:windows"
	cppdialect "C++20"
	--staticruntime "On"
	systemversion "latest"


	defines
	{
		--"SDL_STATIC_LIB",
		--"_WIN64",
		"WLFR_PLATFORM_WINDOWS"
	}




filter "configurations:Debug"
	defines {"WLFR_DEBUG", "_DEBUG"}
	symbols "On"


filter "configurations:Release"
	defines {"WLFR_RELEASE", "NDEBUG"}
	optimize "On"


filter "configurations:Dist"
	defines "WLFR_DIST"
	optimize "On"