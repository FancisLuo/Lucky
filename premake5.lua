workspace "Lucky"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Lucky"
    location "Lucky"
    kind "SharedLib" -- dll
    language "C++"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/entt/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "LK_PLATFORM_WINDOWS",
            "LK_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/Sandbox")
        }

    filter "configurations:Debug"
        defines "LK_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "LK_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "LK_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    language "C++"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "Lucky/vendor/spdlog/include",
        "Lucky/vendor/entt/include",
        "Lucky/src"
    }

    links
    {
        "Lucky"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "LK_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "LK_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "LK_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "LK_DIST"
        optimize "On"
