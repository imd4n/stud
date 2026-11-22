/********************************************************************
 *          ЛАБОРАТОРНАЯ РАБОТА ПО ООП                               *
 *------------------------------------------------------------------*
 * Проект        : Flappy Plane (вариант "Самолёт")                 *
 * Файл          : planeMain.cpp                                    *
 * Комментарий   : Настройка консольного окна, двойная буферизация  *
 *                 и главный цикл игры.                             *
 *******************************************************************/

#include "plane.h"

#include <cstdio>
#include <cwchar>

// Глобальный контекст устройства, через который рисуют все объекты.
HDC hdc = nullptr;

// Привязывает стандартные handles к консоли. Нужно, потому что при
// запуске из IDE/скрипта stdout может быть pipe, а не консоль, и тогда
// SetConsoleScreenBufferSize/SetConsoleWindowInfo молча не работают.
static void BindConsoleHandles()
{
    HANDLE ConOut = CreateFileW(L"CONOUT$",
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                nullptr, OPEN_EXISTING, 0, nullptr);
    if (ConOut != INVALID_HANDLE_VALUE) {
        SetStdHandle(STD_OUTPUT_HANDLE, ConOut);
        SetStdHandle(STD_ERROR_HANDLE, ConOut);
    }

    HANDLE ConIn = CreateFileW(L"CONIN$",
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               nullptr, OPEN_EXISTING, 0, nullptr);
    if (ConIn != INVALID_HANDLE_VALUE) {
        SetStdHandle(STD_INPUT_HANDLE, ConIn);
    }
}

/*------------------------------------------------------------------*/
/*                     НАСТРОЙКА КОНСОЛИ                            */
/*------------------------------------------------------------------*/
static void SetupConsole()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

    // Отключаем QuickEdit: иначе случайный клик мышью "замораживает"
    // консоль и игра перестаёт перерисовываться.
    DWORD Mode = 0;
    if (GetConsoleMode(hIn, &Mode)) {
        Mode |= ENABLE_EXTENDED_FLAGS;
        Mode &= ~ENABLE_QUICK_EDIT_MODE;
        SetConsoleMode(hIn, Mode);
    }

    // Пробуем поставить шрифт покрупнее.
    CONSOLE_FONT_INFOEX Font;
    ZeroMemory(&Font, sizeof(Font));
    Font.cbSize = sizeof(Font);

    if (GetCurrentConsoleFontEx(hOut, FALSE, &Font)) {
        Font.dwFontSize.X = 0;
        Font.dwFontSize.Y = 20;
        Font.FontWeight = FW_NORMAL;
        std::wcscpy(Font.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hOut, FALSE, &Font);
    }

    // Приводим буфер к размеру окна: убираем вертикальный скроллбар,
    // но само окно не трогаем. Если не получилось — работаем как есть.
    CONSOLE_SCREEN_BUFFER_INFO Info;
    if (GetConsoleScreenBufferInfo(hOut, &Info)) {
        SHORT WindowW = Info.srWindow.Right - Info.srWindow.Left + 1;
        SHORT WindowH = Info.srWindow.Bottom - Info.srWindow.Top + 1;

        if (WindowW > 0 && WindowH > 0) {
            COORD Fit = {WindowW, WindowH};
            SetConsoleScreenBufferSize(hOut, Fit);
        }
    }

    COORD BufferSize = {120, 40};
    if (GetConsoleScreenBufferInfo(hOut, &Info)) {
        BufferSize = Info.dwSize;
    }

    int Columns = BufferSize.X;
    int Rows = BufferSize.Y;

    // Красим текстовый буфер в цвет неба: если консоль перерисуется,
    // не будет чёрных вспышек.
    COORD Home = {0, 0};
    DWORD Written = 0;
    DWORD Cells = static_cast<DWORD>(Columns) * static_cast<DWORD>(Rows);

    FillConsoleOutputCharacterW(hOut, L' ', Cells, Home, &Written);
    FillConsoleOutputAttribute(
        hOut,
        BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
        Cells, Home, &Written);
    SetConsoleTextAttribute(
        hOut,
        BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);

    // Прячем курсор.
    CONSOLE_CURSOR_INFO Cursor;
    if (GetConsoleCursorInfo(hOut, &Cursor)) {
        Cursor.bVisible = FALSE;
        SetConsoleCursorInfo(hOut, &Cursor);
    }

    // Ставим окно по центру экрана.
    HWND Hwnd = GetConsoleWindow();
    if (Hwnd != nullptr) {
        RECT R;
        GetWindowRect(Hwnd, &R);

        int WindowW = R.right - R.left;
        int WindowH = R.bottom - R.top;
        int ScreenW = GetSystemMetrics(SM_CXSCREEN);
        int ScreenH = GetSystemMetrics(SM_CYSCREEN);

        int X = (ScreenW - WindowW) / 2;
        int Y = (ScreenH - WindowH) / 2;
        if (X < 0) X = 0;
        if (Y < 0) Y = 0;

        MoveWindow(Hwnd, X, Y, WindowW, WindowH, TRUE);
    }

    SetConsoleTitleW(L"Flappy Plane - OOP Lab");
}

/*------------------------------------------------------------------*/
/*                           ГЛАВНАЯ ФУНКЦИЯ                        */
/*------------------------------------------------------------------*/
int main()
{
    // Если программа запущена без консоли (например, из Qt Creator,
    // который перехватывает вывод), создаём её сами. Иначе GetConsoleWindow
    // вернёт NULL и рисовать будет некуда.
    if (GetConsoleWindow() == nullptr) {
        if (!AllocConsole()) {
            return 1;
        }
        freopen("CONOUT$", "w", stdout);
        freopen("CONIN$", "r", stdin);
    }

    BindConsoleHandles();

    SetupConsole();

    HWND Hwnd = GetConsoleWindow();
    if (Hwnd == nullptr) {
        MessageBoxW(nullptr, L"Console window not found.",
                    L"Flappy Plane", MB_OK | MB_ICONERROR);
        return 1;
    }

    HDC WindowDC = GetDC(Hwnd);
    if (WindowDC == nullptr) {
        MessageBoxW(Hwnd, L"Cannot get window device context.",
                    L"Flappy Plane", MB_OK | MB_ICONERROR);
        return 2;
    }

    RECT Client;
    GetClientRect(Hwnd, &Client);

    int Width = Client.right - Client.left;
    int Height = Client.bottom - Client.top;

    if (Width < 400 || Height < 300) {
        ReleaseDC(Hwnd, WindowDC);
        MessageBoxW(Hwnd, L"Console window is too small for the game.",
                    L"Flappy Plane", MB_OK | MB_ICONERROR);
        return 3;
    }

    // Двойная буферизация: сначала рисуем в память, потом одним
    // BitBlt переносим кадр на экран. Так нет мерцания.
    HDC BackDC = CreateCompatibleDC(WindowDC);
    HBITMAP BackBitmap = CreateCompatibleBitmap(WindowDC, Width, Height);

    if (BackDC == nullptr || BackBitmap == nullptr) {
        if (BackBitmap != nullptr) DeleteObject(BackBitmap);
        if (BackDC != nullptr) DeleteDC(BackDC);
        ReleaseDC(Hwnd, WindowDC);
        MessageBoxW(Hwnd, L"Cannot create back buffer.",
                    L"Flappy Plane", MB_OK | MB_ICONERROR);
        return 4;
    }

    HGDIOBJ OldBitmap = SelectObject(BackDC, BackBitmap);
    hdc = BackDC;

    Game game(Width, Height);
    game.Reset();

    bool Exit = false;
    bool Prev[256] = {};

    const int Keys[] = { VK_ESCAPE, VK_SPACE, VK_UP, 'W', 'R', VK_F1 };
    const int KeyCount = static_cast<int>(sizeof(Keys) / sizeof(Keys[0]));

    while (!Exit) {
        // Опрос клавиатуры с защитой от автоповтора: действие
        // выполняется только в момент нажатия.
        for (int Index = 0; Index < KeyCount; Index++) {
            int Key = Keys[Index];
            bool Down = (GetAsyncKeyState(Key) & 0x8000) != 0;
            bool Pressed = Down && !Prev[Key];
            Prev[Key] = Down;

            if (!Pressed) continue;

            if (Key == VK_ESCAPE) {
                Exit = true;
            } else if (Key == VK_F1) {
                game.ToggleHitBoxes();
            } else {
                game.PressFlap();
            }
        }

        if (Exit) break;

        game.Update();
        game.Render(BackDC);

        BitBlt(WindowDC, 0, 0, Width, Height, BackDC, 0, 0, SRCCOPY);

        Sleep(16);
    }

    hdc = nullptr;

    SelectObject(BackDC, OldBitmap);
    DeleteObject(BackBitmap);
    DeleteDC(BackDC);
    ReleaseDC(Hwnd, WindowDC);

    return 0;
}
