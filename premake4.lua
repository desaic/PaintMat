solution "PaintMat"
  configurations { "Debug", "Release" }
  
  configuration { "Debug" }
    targetdir "bin/debug"
    buildoptions{"-std=c++0x"}
 
  configuration { "Release" }
    targetdir "bin/release"
    buildoptions{"-std=c++0x"}
 
project "PaintMat"
  language "C++"
  kind     "ConsoleApp"
  files  { "src/**.cpp" }
  includedirs {"./include/"}
  --For windows
  --this directory should be changed to windows specific directory
  if os.is("Windows") then 
    links {"./lib/win32/png"}
  else
    links {"png", "GL","GLU","glut","gomp","m"}
  end
  
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
