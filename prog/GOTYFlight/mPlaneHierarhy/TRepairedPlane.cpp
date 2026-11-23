#include "TRepairedPlane.h"
#include "TBrokenPlane.h"

#include <wingdi.h>

extern HDC hdc;

void RepairedPlane::fuselage(bool f) {
  // Установка сплошного пера для целых частей самолета - СИНИЙ основной цвет
  HPEN solidPen;
  if (f) solidPen = CreatePen(PS_SOLID, 2, RGB(0, 100, 255));
  else solidPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
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

  // ЗЕЛЕНЫЕ ЗАПЛАТКИ И СЛЕДЫ РЕМОНТА:

  // 1. Заплатка на корпусе (ярко-зеленая)
  HPEN patchPen;
  if (f) patchPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
  else patchPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
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
  HPEN rivetPen;
  if (f) rivetPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 0));
  else rivetPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
  SelectObject(hdc, rivetPen);

  // 5. ОРАНЖЕВЫЕ НОВЫЕ ДЕТАЛИ
  HPEN newPartPen;
  if (f) newPartPen = CreatePen(PS_SOLID, 2, RGB(255, 165, 0));
  else newPartPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
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
  HPEN windowPen;
  if (f) windowPen = CreatePen(PS_SOLID, 2, RGB(128, 0, 128));
  else windowPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, windowPen);
  MoveToEx(hdc, x + 5, y + 5, NULL);
  LineTo(hdc, x + 8, y + 8);
  LineTo(hdc, x + 5, y + 12);
  LineTo(hdc, x + 2, y + 8);
  LineTo(hdc, x + 5, y + 5);

  // 7. ЯРКИЕ КРАСНЫЕ ЛИНИИ НА КРЫЛЬЯХ
  HPEN stripePen;
  if (f) stripePen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
  else stripePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
  SelectObject(hdc, stripePen);

  // Полосы на крыльях
  MoveToEx(hdc, x + 15, y + 15, NULL);
  LineTo(hdc, x + 40, y + 40);

  MoveToEx(hdc, x - 15, y + 15, NULL);
  LineTo(hdc, x - 40, y + 40);

  // 8. ЗЕЛЕНЫЙ СВЕТОДИОД - индикатор исправности
  HPEN ledPen;
  if (f) ledPen = CreatePen(PS_SOLID, 4, RGB(0, 255, 0));
  else ledPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
  SelectObject(hdc, ledPen);

  DeleteObject(solidPen);
  DeleteObject(patchPen);
  DeleteObject(rivetPen);
  DeleteObject(newPartPen);
  DeleteObject(windowPen);
  DeleteObject(stripePen);
  DeleteObject(ledPen);
}

void RepairedPlane::wings(bool f) {
  HPEN solidPen;
  if (f) solidPen = CreatePen(PS_SOLID, 2, RGB(0, 100, 255));
  else solidPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, solidPen);


  // Полные крылья
  MoveToEx(hdc, x + 25, y + 35, NULL);
  LineTo(hdc, x + 50, y + 45); // Полное правое крыло

  MoveToEx(hdc, x - 25, y + 35, NULL);
  LineTo(hdc, x - 50, y + 45); // Полное левое крыло

  DeleteObject(solidPen);
}

void RepairedPlane::hvost(bool f) {
  HPEN solidPen;
  if (f) solidPen = CreatePen(PS_SOLID, 2, RGB(0, 100, 255));
  else solidPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, solidPen);


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
  HPEN gearPen;
  if (f) gearPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  else gearPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, gearPen);

  MoveToEx(hdc, x + 10, y + 55, NULL);
  LineTo(hdc, x + 10, y + 65);
  MoveToEx(hdc, x - 10, y + 55, NULL);
  LineTo(hdc, x - 10, y + 65);

  // Колеса - ЧЕРНЫЕ
  HPEN wheelPen;
  if (f) wheelPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
  else wheelPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
  SelectObject(hdc, wheelPen);

  MoveToEx(hdc, x + 10, y + 65, NULL);
  LineTo(hdc, x + 20, y + 68);
  MoveToEx(hdc, x - 10, y + 65, NULL);
  LineTo(hdc, x - 20, y + 68);

  DeleteObject(solidPen);
  DeleteObject(gearPen);
  DeleteObject(wheelPen);
}

RepairedPlane::RepairedPlane(int _x, int _y) : BrokenPlane(_x, _y) {
  setHeight(80);
  setWidth(120);
  setCollisionX(-70);
  setCollisionY(0);

  ID = 4;
}

RepairedPlane::~RepairedPlane() {}