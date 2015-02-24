cmake_minimum_required(VERSION 2.8.4)
project(ShoebillPlugin)
option(SHOEBILL_USE_STATIC_RUNTIME "Use static C++ runtime" OFF)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Enable project folders in Visual Studio 2010+ (and possibly other IDEs).
if(NOT (MSVC_IDE AND MSVC_VERSION LESS 1600))
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Suppress annoying GCC warning about 'packed' attribute being ignored.
if(CMAKE_COMPILER_IS_GNUCC)
  if(NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 4.0)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-attributes")
  endif()
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
  endif()
endif()

# Link against the static version of libstdc++.
if(SHOEBILL_USE_STATIC_RUNTIME)
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
  endif()
endif()

# Disalow the use of SSE2 instruction set on Windows. Some people still have
# old CPUs on their home PCs that do not support SSE2 and hosts typically use
# modern CPUs and mostly run Linux.
if(WIN32)
  add_definitions(-DWIN32)
  if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE")
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=pentium3m")
  endif()
endif()

if(UNIX)
add_definitions(-DLINUX)
endif()

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -O3 -Wall -ansi -std=c++11 -fPIC -fno-strict-aliasing -Wno-write-strings -fshort-wchar -static-libstdc++")
 endif()

add_subdirectory(src)
