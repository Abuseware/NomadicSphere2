cmake_minimum_required(VERSION 3.13)

if(APPLE)
    set(HAVE_FLAG_SEARCH_PATHS_FIRST 0) # Fixup for MacOS MinGW
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32 OR MINGW)
    add_compile_definitions(PATH_SEPARATOR='\\\\')
else()
    add_compile_definitions(PATH_SEPARATOR='/')
endif()

project(ImageSearch2)
add_executable(${PROJECT_NAME} main.cc Image.cc Database.cc)