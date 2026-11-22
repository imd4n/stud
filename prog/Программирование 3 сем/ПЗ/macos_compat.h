#ifndef MACOS_COMPAT_H
#define MACOS_COMPAT_H

#ifdef __APPLE__
#include <SDL2/SDL.h>

// Определяем типы, которые используются в Windows
typedef void* HWND;
typedef void* HDC;

// Заменим HDC на структуру с SDL-рендерером
struct MacOSHDC {
    SDL_Renderer* renderer;
    SDL_Window* window;
};

#define RGB(r, g, b) ((0 << 24) | ((b) << 16) | ((g) << 8) | (r))

// Прототипы функций
void InitSDL();
void QuitSDL();
void SetPixel(HDC hdc, int x, int y, unsigned long color);

// Объявляем функции, которые эмулируют Windows API
HWND GetConsoleWindow();
HDC GetWindowDC(HWND hwnd);
void SetPixel(HDC hdc, int x, int y, unsigned long color);

#else
// Оригинальные Windows-заголовки
#include <windows.h>
#endif

#endif