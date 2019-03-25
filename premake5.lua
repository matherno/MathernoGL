workspace "MathernoGL"
   configurations { "debug", "release" }

project "MathernoGL"
   kind "StaticLib"
   language "C++"
   targetdir "bin_%{cfg.buildcfg}"
   
   files { "include/**.h", "src/**.cpp" }

   links { "GL", "GLEW", "glfw", "freeimage" }
   
   includedirs { "./dependencies/include", "./include/mathernogl", "./include", "./src" }

   cppdialect "C++14"

   filter "configurations:debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"