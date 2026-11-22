set(CMAKE_SYSTEM_NAME Windows)              # Целевая система
set(CMAKE_SYSTEM_PROCESSOR x86_64)          # Целевая архитектура

set(CMAKE_C_COMPILER /opt/homebrew/bin/x86_64-w64-mingw32-gcc)        # Компилятор C для кросс-компиляции
set(CMAKE_CXX_COMPILER /opt/homebrew/bin/x86_64-w64-mingw32-g++)      # Компилятор C++ для кросс-компиляции

set(CMAKE_FIND_ROOT_PATH /opt/homebrew/opt/mingw-w64)    # Пути поиска библиотек и заголовков

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)     # Искать программы на хосте, а не в целевой системе
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)      # Искать библиотеки только в целевых путях
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)      # Искать заголовки только в целевых путях