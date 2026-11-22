#include "TRepairedPlane.h"
#include "TBrokenPlane.h"

#include <wingdi.h>

extern HDC hdc;

RepairedPlane::RepairedPlane(int _x, int _y) : BrokenPlane(_x, _y) {
  setHeight(80);
  setWidth(120);
  setCollisionX(-70);
  setCollisionY(0);

}

RepairedPlane::~RepairedPlane() {}

void RepairedPlane::show() {
// Установка сплошного пера для целых частей самолета - СИНИЙ основной цвет
HPEN solidPen = CreatePen(PS_SOLID, 2, RGB(0, 100, 255));
SelectObject(hdc, solidPen);

// Основной корпус самолета - восстановлен
MoveToEx(hdc, x, y, NULL);
LineTo(hdc, x + 25, y + 12); // Полное правое крыло

MoveToEx(hdc, x, y, NULL);
LineTo(hdc, x - 25, y + 12); // Полное левое крыло

// Корпус без изгибов
MoveToEx(hdc, x + 25, y + 12, NULL);
LineTo(hdc, x + 25, y + 35);
MoveToEx(hdc, x - 25, y + 12, NULL);
LineTo(hdc, x - 25, y + 35);

// Полные крылья
MoveToEx(hdc, x + 25, y + 35, NULL);
LineTo(hdc, x + 50, y + 45); // Полное правое крыло

MoveToEx(hdc, x - 25, y + 35, NULL);
LineTo(hdc, x - 50, y + 45); // Полное левое крыло

// Хвостовая часть восстановлена
MoveToEx(hdc, x + 50, y + 45, NULL);
LineTo(hdc, x + 30, y + 55);
MoveToEx(hdc, x - 50, y + 45, NULL);
LineTo(hdc, x - 30, y + 55);

MoveToEx(hdc, x + 30, y + 55, NULL);
LineTo(hdc, x + 10, y + 55);
MoveToEx(hdc, x - 30, y + 55, NULL);
LineTo(hdc, x - 10, y + 55);

// Исправное шасси - КРАСНЫЕ стойки
HPEN gearPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
SelectObject(hdc, gearPen);

MoveToEx(hdc, x + 10, y + 55, NULL);
LineTo(hdc, x + 10, y + 65);
MoveToEx(hdc, x - 10, y + 55, NULL);
LineTo(hdc, x - 10, y + 65);

// Колеса - ЧЕРНЫЕ
HPEN wheelPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
SelectObject(hdc, wheelPen);

MoveToEx(hdc, x + 10, y + 65, NULL);
LineTo(hdc, x + 20, y + 68);
MoveToEx(hdc, x - 10, y + 65, NULL);
LineTo(hdc, x - 20, y + 68);

// ЗЕЛЕНЫЕ ЗАПЛАТКИ И СЛЕДЫ РЕМОНТА:

// 1. Заплатка на корпусе (ярко-зеленая)
HPEN patchPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
SelectObject(hdc, patchPen);

// Заплатка в месте бывшей трещины
MoveToEx(hdc, x + 3, y + 18, NULL);
LineTo(hdc, x + 8, y + 22);
LineTo(hdc, x + 5, y + 28);
LineTo(hdc, x, y + 24);
LineTo(hdc, x + 3, y + 18);

// 2. Заплатка на правом крыле
MoveToEx(hdc, x + 35, y + 30, NULL);
LineTo(hdc, x + 40, y + 33);
LineTo(hdc, x + 38, y + 38);
LineTo(hdc, x + 33, y + 35);
LineTo(hdc, x + 35, y + 30);

// 3. Заплатка на левом крыле
MoveToEx(hdc, x - 35, y + 32, NULL);
LineTo(hdc, x - 40, y + 35);
LineTo(hdc, x - 38, y + 40);
LineTo(hdc, x - 33, y + 37);
LineTo(hdc, x - 35, y + 32);

// 4. ЯРКИЕ ЗАКЛЕПКИ (желтые)
HPEN rivetPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 0));
SelectObject(hdc, rivetPen);

// Заклепки вокруг заплаток
SetPixel(hdc, x + 5, y + 20, RGB(255, 255, 0));
SetPixel(hdc, x + 2, y + 22, RGB(255, 255, 0));
SetPixel(hdc, x + 7, y + 25, RGB(255, 255, 0));

SetPixel(hdc, x + 37, y + 32, RGB(255, 255, 0));
SetPixel(hdc, x + 39, y + 35, RGB(255, 255, 0));

SetPixel(hdc, x - 37, y + 34, RGB(255, 255, 0));
SetPixel(hdc, x - 39, y + 37, RGB(255, 255, 0));

// 5. ОРАНЖЕВЫЕ НОВЫЕ ДЕТАЛИ
HPEN newPartPen = CreatePen(PS_SOLID, 2, RGB(255, 165, 0));
SelectObject(hdc, newPartPen);

// Новая законцовка левого крыла
MoveToEx(hdc, x - 50, y + 45, NULL);
LineTo(hdc, x - 55, y + 43);
LineTo(hdc, x - 53, y + 47);
LineTo(hdc, x - 50, y + 45);

// Новая законцовка правого крыла
MoveToEx(hdc, x + 50, y + 45, NULL);
LineTo(hdc, x + 55, y + 43);
LineTo(hdc, x + 53, y + 47);
LineTo(hdc, x + 50, y + 45);

// 6. ФИОЛЕТОВОЕ ОКНО КАБИНЫ
HPEN windowPen = CreatePen(PS_SOLID, 2, RGB(128, 0, 128));
SelectObject(hdc, windowPen);
MoveToEx(hdc, x + 5, y + 5, NULL);
LineTo(hdc, x + 8, y + 8);
LineTo(hdc, x + 5, y + 12);
LineTo(hdc, x + 2, y + 8);
LineTo(hdc, x + 5, y + 5);

// 7. ЯРКИЕ КРАСНЫЕ ЛИНИИ НА КРЫЛЬЯХ
HPEN stripePen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
SelectObject(hdc, stripePen);

// Полосы на крыльях
MoveToEx(hdc, x + 15, y + 15, NULL);
LineTo(hdc, x + 40, y + 40);

MoveToEx(hdc, x - 15, y + 15, NULL);
LineTo(hdc, x - 40, y + 40);

// 8. ЗЕЛЕНЫЙ СВЕТОДИОД - индикатор исправности
HPEN ledPen = CreatePen(PS_SOLID, 4, RGB(0, 255, 0));
SelectObject(hdc, ledPen);
SetPixel(hdc, x - 5, y + 40, RGB(0, 255, 0));

// Очистка объектов GDI
DeleteObject(solidPen);
DeleteObject(gearPen);
DeleteObject(wheelPen);
DeleteObject(patchPen);
DeleteObject(rivetPen);
DeleteObject(newPartPen);
DeleteObject(windowPen);
DeleteObject(stripePen);
DeleteObject(ledPen);
}






void RepairedPlane::hide() {
// Установка сплошного пера для целых частей самолета - СИНИЙ основной цвет
HPEN solidPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
SelectObject(hdc, solidPen);

// Основной корпус самолета - восстановлен
MoveToEx(hdc, x, y, NULL);
LineTo(hdc, x + 25, y + 12); // Полное правое крыло

MoveToEx(hdc, x, y, NULL);
LineTo(hdc, x - 25, y + 12); // Полное левое крыло

// Корпус без изгибов
MoveToEx(hdc, x + 25, y + 12, NULL);
LineTo(hdc, x + 25, y + 35);
MoveToEx(hdc, x - 25, y + 12, NULL);
LineTo(hdc, x - 25, y + 35);

// Полные крылья
MoveToEx(hdc, x + 25, y + 35, NULL);
LineTo(hdc, x + 50, y + 45); // Полное правое крыло

MoveToEx(hdc, x - 25, y + 35, NULL);
LineTo(hdc, x - 50, y + 45); // Полное левое крыло

// Хвостовая часть восстановлена
MoveToEx(hdc, x + 50, y + 45, NULL);
LineTo(hdc, x + 30, y + 55);
MoveToEx(hdc, x - 50, y + 45, NULL);
LineTo(hdc, x - 30, y + 55);

MoveToEx(hdc, x + 30, y + 55, NULL);
LineTo(hdc, x + 10, y + 55);
MoveToEx(hdc, x - 30, y + 55, NULL);
LineTo(hdc, x - 10, y + 55);

// Исправное шасси - КРАСНЫЕ стойки
HPEN gearPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
SelectObject(hdc, gearPen);

MoveToEx(hdc, x + 10, y + 55, NULL);
LineTo(hdc, x + 10, y + 65);
MoveToEx(hdc, x - 10, y + 55, NULL);
LineTo(hdc, x - 10, y + 65);

// Колеса - ЧЕРНЫЕ
HPEN wheelPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
SelectObject(hdc, wheelPen);

MoveToEx(hdc, x + 10, y + 65, NULL);
LineTo(hdc, x + 20, y + 68);
MoveToEx(hdc, x - 10, y + 65, NULL);
LineTo(hdc, x - 20, y + 68);

// ЗЕЛЕНЫЕ ЗАПЛАТКИ И СЛЕДЫ РЕМОНТА:

// 1. Заплатка на корпусе (ярко-зеленая)
HPEN patchPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
SelectObject(hdc, patchPen);

// Заплатка в месте бывшей трещины
MoveToEx(hdc, x + 3, y + 18, NULL);
LineTo(hdc, x + 8, y + 22);
LineTo(hdc, x + 5, y + 28);
LineTo(hdc, x, y + 24);
LineTo(hdc, x + 3, y + 18);

// 2. Заплатка на правом крыле
MoveToEx(hdc, x + 35, y + 30, NULL);
LineTo(hdc, x + 40, y + 33);
LineTo(hdc, x + 38, y + 38);
LineTo(hdc, x + 33, y + 35);
LineTo(hdc, x + 35, y + 30);

// 3. Заплатка на левом крыле
MoveToEx(hdc, x - 35, y + 32, NULL);
LineTo(hdc, x - 40, y + 35);
LineTo(hdc, x - 38, y + 40);
LineTo(hdc, x - 33, y + 37);
LineTo(hdc, x - 35, y + 32);

// 4. ЯРКИЕ ЗАКЛЕПКИ (желтые)
HPEN rivetPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
SelectObject(hdc, rivetPen);

// Заклепки вокруг заплаток
SetPixel(hdc, x + 5, y + 20, RGB(0, 0, 0));
SetPixel(hdc, x + 2, y + 22, RGB(0, 0, 0));
SetPixel(hdc, x + 7, y + 25, RGB(0, 0, 0));

SetPixel(hdc, x + 37, y + 32, RGB(0, 0, 0));
SetPixel(hdc, x + 39, y + 35, RGB(0, 0, 0));

SetPixel(hdc, x - 37, y + 34, RGB(0, 0, 0));
SetPixel(hdc, x - 39, y + 37, RGB(0, 0, 0));

// 5. ОРАНЖЕВЫЕ НОВЫЕ ДЕТАЛИ
HPEN newPartPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
SelectObject(hdc, newPartPen);

// Новая законцовка левого крыла
MoveToEx(hdc, x - 50, y + 45, NULL);
LineTo(hdc, x - 55, y + 43);
LineTo(hdc, x - 53, y + 47);
LineTo(hdc, x - 50, y + 45);

// Новая законцовка правого крыла
MoveToEx(hdc, x + 50, y + 45, NULL);
LineTo(hdc, x + 55, y + 43);
LineTo(hdc, x + 53, y + 47);
LineTo(hdc, x + 50, y + 45);

// 6. ФИОЛЕТОВОЕ ОКНО КАБИНЫ
HPEN windowPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
SelectObject(hdc, windowPen);
MoveToEx(hdc, x + 5, y + 5, NULL);
LineTo(hdc, x + 8, y + 8);
LineTo(hdc, x + 5, y + 12);
LineTo(hdc, x + 2, y + 8);
LineTo(hdc, x + 5, y + 5);

// 7. ЯРКИЕ КРАСНЫЕ ЛИНИИ НА КРЫЛЬЯХ
HPEN stripePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
SelectObject(hdc, stripePen);

// Полосы на крыльях
MoveToEx(hdc, x + 15, y + 15, NULL);
LineTo(hdc, x + 40, y + 40);

MoveToEx(hdc, x - 15, y + 15, NULL);
LineTo(hdc, x - 40, y + 40);

// 8. ЗЕЛЕНЫЙ СВЕТОДИОД - индикатор исправности
HPEN ledPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
SelectObject(hdc, ledPen);
SetPixel(hdc, x - 5, y + 40, RGB(0, 255, 0));

// Очистка объектов GDI
DeleteObject(solidPen);
DeleteObject(gearPen);
DeleteObject(wheelPen);
DeleteObject(patchPen);
DeleteObject(rivetPen);
DeleteObject(newPartPen);
DeleteObject(windowPen);
DeleteObject(stripePen);
DeleteObject(ledPen);
}