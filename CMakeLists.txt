cmake_minimum_required(VERSION 3.20)

project(
        ChartLabelEngine
        VERSION 0.1.0
        DESCRIPTION "A C++ library for automatic chart label placement"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(chart_label_engine STATIC
        src/chart_label_engine.cpp
)

target_include_directories(chart_label_engine
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

