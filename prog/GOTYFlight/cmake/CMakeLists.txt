cmake_minimum_required(VERSION 3.20)
project(GOTYFlight CXX)

# Папки с заголовками
include_directories(${PROJECT_NAME}
    ../base
    ../objects
    ../planes
    ../towers
)

add_executable(${PROJECT_NAME}
    # Основной файл программы
    ../main.cpp

    # Базовые классы
    ../base/Location.cpp
    ../base/Point.cpp

    # Самолёты
    ../planes/Plane.cpp
    ../planes/MagicPlane.cpp
    ../planes/BrokenPlane.cpp

    # Объекты
    ../objects/Rockets.cpp

    # Башни
    ../towers/Tower.cpp
    ../towers/MagicTower.cpp
)

# Установка выходной директории
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../output
)

# Явное указание флагов для целевого объекта
target_compile_options(${PROJECT_NAME} PRIVATE
    -static
)

target_link_options(${PROJECT_NAME} PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
)
