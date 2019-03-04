cmake_minimum_required(VERSION 3.13)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0) # Fixup for MacOS MinGW
project(ImageSearch2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-pipe -march=core2 -mtune=generic -fPIC -fPIE -static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,-O1")

if (WIN32 OR MINGW)
    set(MINGW True)
    add_compile_definitions(PATH_SEPARATOR='\\\\')
else ()
    add_compile_definitions(PATH_SEPARATOR='/')
endif ()

add_executable(ImageSearch2 main.cc Image.cc Image.h Database.cc Database.h)