#ifdef __APPLE__
#include "macos_compat.h"
#include <iostream>

static SDL_Window* main_window = nullptr;
static SDL_Renderer* main_renderer = nullptr;

void InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    main_window = SDL_CreateWindow("Windows App Compatibility Layer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
        
    main_renderer = SDL_CreateRenderer(main_window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void QuitSDL() {
    if (main_renderer) SDL_DestroyRenderer(main_renderer);
    if (main_window) SDL_DestroyWindow(main_window);
    SDL_Quit();
}

void SetPixel(HDC hdc, int x, int y, unsigned long color) {
    SDL_SetRenderDrawColor(hdc->renderer,
        color & 0xFF,           // R
        (color >> 8) & 0xFF,    // G
        (color >> 16) & 0xFF,   // B
        255);                   // Alpha
    SDL_RenderDrawPoint(hdc->renderer, x, y);
}

// Реализация GetConsoleWindow для macOS
HWND GetConsoleWindow() {
    static bool initialized = false;
    if (!initialized) {
        InitSDL();
        initialized = true;
    }
    
    static MacOSHDC hdc = {main_renderer, main_window};
    return &hdc;
}

// Реализация GetWindowDC для macOS
extern "C" void* GetWindowDC(void* hwnd) {
    return hwnd; // Просто возвращаем переданный указатель
}

// Реализация SetPixel для macOS
HWND SetPixel(void* hdc, int x, int y, unsigned long color) {
    SetPixel((HDC)hdc, x, y, color);
    SDL_RenderPresent(((HDC)hdc)->renderer);
    return color;
}

#endif