workspace "WolfRenderer"

	architecture "x64"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "WolfRenderer"

	location "WolfRenderer"

	kind "SharedLib"

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
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src/core/include"
	}


	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"


defines
{
	"WLFR_PLATFORM_WINDOWS", 
	"WLFR_BUILD_DLL"
}

postbuildcommands
{
	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
}

filter "configurations:Debug"
	defines "WLFR_DEBUG"
	symbols "On"

filter "configurations:Release"
	defines "WLFR_RELEASE"
	optimize "On"

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
		"WolfRenderer/vendor/spdlog/include",
		"WolfRenderer/src",
		"WolfRenderer/src/core/include"
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

filter "configurations:Release"
	defines "WLFR_RELEASE"
	optimize "On"

filter "configurations:Dist"
	defines "WLFR_DIST"
	optimize "On"