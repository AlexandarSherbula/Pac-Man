workspace "Pac-Man"
   configurations { "Debug", "Release" }

project "Pac-Man"
   location "Pac-Man"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "On"
   
   targetdir "build/%{prj.name}/bin/%{cfg.system}/%{cfg.buildcfg}-%{cfg.architecture}"
   objdir    "build/%{prj.name}/intermediate/%{cfg.system}/%{cfg.buildcfg}-%{cfg.architecture}"
   
   files
   {
      "Pac-Man/src/**.h",
      "Pac-Man/src/**.cpp"
   }

   filter "configurations:Debug"
      symbols "On"
   
   filter "configurations:Release"
      optimize "On"

   filter "system:windows"
      staticruntime "On"
      systemversion "latest"