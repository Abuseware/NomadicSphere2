cmake_minimum_required(VERSION 3.13)

set(HAVE_FLAG_SEARCH_PATHS_FIRST 0) # Fixup for MacOS MinGW

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-pipe -march=core2 -mtune=generic -fPIC -fPIE")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

if (WIN32 OR MINGW)
    add_compile_definitions(PATH_SEPARATOR='\\\\')
else ()
    add_compile_definitions(PATH_SEPARATOR='/')
endif ()

project(ImageSearch2)

if (MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif ()


add_executable(ImageSearch2 main.cc Image.cc Image.h Database.cc Database.h)