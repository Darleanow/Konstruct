cmake_minimum_required(VERSION 3.28)
project(Konstruct)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include(FetchContent)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Konstruct main.cpp)

target_link_libraries(Konstruct PUBLIC yaml-cpp::yaml-cpp)

target_sources(Konstruct
        PRIVATE
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS include
        FILES
        include/ParseOption.ixx
        include/ConfHandler.ixx
        include/Token.ixx
        include/Utility.ixx
        include/BaseHandler.ixx
        include/REPLHandler.ixx
        include/CLIHandler.ixx
)

if (MSVC)
    target_compile_options(Konstruct PRIVATE /experimental:module /std:c++latest)
endif ()
