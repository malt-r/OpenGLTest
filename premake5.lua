-- premake5.lua

-- workspace analog to solution
workspace "OpenGLTest"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

-- dynamic outputdir
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "OpenGLTest/vendor/GLFW/include"

include "OpenGLTest/vendor/GLFW"

project "OpenGLTest"
    -- relative path
    location "OpenGLTest"
    kind "ConsoleApp"
    language "C++"

    -- binary outputs
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")

    -- intermediate outputs
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }


    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
