cmake_minimum_required(VERSION 3.28)
project(Konstruct)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_executable(Konstruct main.cpp)

target_sources(Konstruct
        PRIVATE
        FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS include
        FILES
        include/OptionTypes.ixx
        include/ParseOption.ixx
        include/REPL.ixx
        include/Parser.ixx
)

if (MSVC)
    target_compile_options(Konstruct PRIVATE /experimental:module /std:c++latest)
endif ()
