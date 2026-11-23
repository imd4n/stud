cmake_minimum_required(VERSION 3.20)
project(GOTYFlight CXX)


# Добавление исполняемого файла
include_directories(${PROJECT_NAME}
    ../mBaseHierarhy # Базовая иерархия
    ../mDrawingHierarhy # Элементы рисования
)

add_executable(${PROJECT_NAME}
    # Основной файл программы
    ../main.cpp

    # Базовые класссы
    ../mBaseHierarhy/TLocation.cpp
    ../mBaseHierarhy/TPoint.cpp

    # Иерархия 1
    ../mPlaneHierarhy/TPlane.cpp
    ../mPlaneHierarhy/TMagicPlane.cpp
    ../mPlaneHierarhy/TBrokenPlane.cpp
    ../mPlaneHierarhy/TRepairedPlane.cpp

    ../mObjects/TRepairZone.cpp
    ../mObjects/TRockets.cpp
    ../mObjects/TRocketsFill.cpp
    
    # Иерархия 2
    ../mTowerHierarhy/TTower.cpp
    ../mTowerHierarhy/TMagicTower.cpp

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