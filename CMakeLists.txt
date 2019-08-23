cmake_minimum_required(VERSION 3.13)

if(APPLE)
    set(HAVE_FLAG_SEARCH_PATHS_FIRST 0) # Fixup for MacOS MinGW
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -municode")
endif ()

#if(WIN32 OR MINGW)
#    add_compile_definitions(PATH_SEPARATOR='\\\\')
#else()
#    add_compile_definitions(PATH_SEPARATOR='/')
#endif()


project(ImageSearch2)
add_executable(${PROJECT_NAME} main.cc Image.cc Database.cc)

#Catch2 test
if (CMAKE_BUILD_TYPE MATCHES Debug)
    find_package("Catch2" REQUIRED)
    add_executable(run_test test.cc Image.cc Database.cc)
    target_link_libraries(run_test Catch2::Catch2)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/data")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test_data/1.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/data")
endif ()