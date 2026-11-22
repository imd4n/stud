cmake_minimum_required(VERSION 3.16)

project(PlaneSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(PlaneSim
    planeMain.cpp
    plane.cpp
)

target_include_directories(PlaneSim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(PlaneSim PRIVATE gdi32 user32)
