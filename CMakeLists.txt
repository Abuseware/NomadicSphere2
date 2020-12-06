cmake_minimum_required(VERSION 3.13)

if (APPLE)
    set(HAVE_FLAG_SEARCH_PATHS_FIRST 0) # Fixup for MacOS MinGW
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DFORTIFY_SOURCE=2 -pipe -flto -fPIC -fPIE")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,-O1,-s")

project(ImageSearch2)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} main.cc Image.cc Database.cc)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
if (MINGW)
    target_link_libraries(${PROJECT_NAME} -static -static-libgcc -static-libstdc++)
endif ()

#Catch2 test
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_executable(run_test test.cc Image.cc Database.cc)
    target_link_libraries(run_test ${CONAN_LIBS})
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/data")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test_data/1.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/data")
endif ()