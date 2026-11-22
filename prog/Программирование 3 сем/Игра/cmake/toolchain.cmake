# original setup: cross-compile to Windows from macOS (Homebrew MinGW-w64)
if(CMAKE_HOST_APPLE)
    set(CMAKE_SYSTEM_NAME Windows)              # целевая система
    set(CMAKE_SYSTEM_PROCESSOR x86_64)          # целевая архитектура

    set(CMAKE_C_COMPILER /opt/homebrew/bin/x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER /opt/homebrew/bin/x86_64-w64-mingw32-g++)

    set(CMAKE_FIND_ROOT_PATH /opt/homebrew/opt/mingw-w64)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)     # программы ищем на хосте
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)      # библиотеки — в целевых путях
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)      # заголовки — в целевых путях

# windows setup: native MinGW build (used by the Zed F5 task)
elseif(CMAKE_HOST_WIN32)
    find_program(_IGRA_GXX NAMES g++ x86_64-w64-mingw32-g++)
    find_program(_IGRA_GCC NAMES gcc x86_64-w64-mingw32-gcc)
    if(NOT _IGRA_GXX)
        message(FATAL_ERROR "g++ not found. Add MinGW-w64 bin to PATH.")
    endif()
    set(CMAKE_CXX_COMPILER "${_IGRA_GXX}")
    if(_IGRA_GCC)
        set(CMAKE_C_COMPILER "${_IGRA_GCC}")
    endif()
endif()
