solution "PaintMat"
  configurations { "Debug", "Release" }
  
  configuration { "Debug" }
    targetdir "debug"
    buildoptions{"-std=c++0x"}
 
  configuration { "Release" }
    targetdir "release"
    buildoptions{"-std=c++0x"}
  include("extern/vecmath")
project "PaintMat"
  language "C++"
  kind     "ConsoleApp"
  files  { "src/**.cpp" }
  includedirs {"./include/","extern/include","extern/include/Eigen"}
  --For windows
  --this directory should be changed to windows specific directory
  if os.is("Windows") then 
    links {"./lib/win32/png"}
  else
    links {"png", "GL","GLU","glut","gomp","m","pthread"}
  end
  links {"vecmath"}
  if os.is("macosx") then
    libdirs{"/opt/local/lib"}
    includedirs{"/opt/local/include"}
  end 
  
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" } 
