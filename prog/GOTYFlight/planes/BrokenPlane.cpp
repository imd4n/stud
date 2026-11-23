#include "BrokenPlane.h"
#include <wingdi.h>

extern HDC hdc;

void BrokenPlane::fuselage(bool f) {
  // Установка прерывистого пера для эффекта повреждений
  HPEN brokenPen;
  if (f) brokenPen = CreatePen(PS_DASH, 2, RGB(100, 100, 100));
  else brokenPen = CreatePen(PS_DASH, 2, RGB(0, 0, 0));
  SelectObject(hdc, brokenPen);
  
  // Основной корпус самолета с повреждениями
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + 12, y + 12); // Укороченное крыло

  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x - 8, y + 12); // Частично отломанное левое крыло

  // Трещина в корпусе
  MoveToEx(hdc, x + 5, y + 8, NULL);
  LineTo(hdc, x - 5, y + 15);

  // Корпус с изгибом
  MoveToEx(hdc, x + 12, y + 12, NULL);
  LineTo(hdc, x + 12, y + 35);
  MoveToEx(hdc, x - 8, y + 12, NULL);
  LineTo(hdc, x - 8, y + 35);

  // ЭЛЕМЕНТЫ ПОВРЕЖДЕНИЙ:

  // 1. Трещины на корпусе
  HPEN crackPen;
  if (f) crackPen = CreatePen(PS_SOLID, 1, RGB(150, 0, 0));
  else crackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hdc, crackPen);

  // Большая трещина на корпусе
  MoveToEx(hdc, x + 5, y + 20, NULL);
  LineTo(hdc, x + 8, y + 25);
  LineTo(hdc, x + 3, y + 30);
  LineTo(hdc, x + 7, y + 35);

  // Трещина на правом крыле
  MoveToEx(hdc, x + 20, y + 25, NULL);
  LineTo(hdc, x + 22, y + 28);
  LineTo(hdc, x + 18, y + 32);

  // 2. Отсутствующие детали - рисуем прерывистые линии
  HPEN missingPen;
  if (f) missingPen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
  else missingPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hdc, missingPen);

  // Отсутствующий кусок правого крыла
  MoveToEx(hdc, x + 30, y + 45, NULL);
  LineTo(hdc, x + 45, y + 55);
  MoveToEx(hdc, x + 45, y + 55, NULL);
  LineTo(hdc, x + 40, y + 65);

  // 3. Дым из поврежденного двигателя
  HPEN smokePen;
  if (f) smokePen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
  else smokePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hdc, smokePen);

  // Волнистые линии дыма
  int smokeX = x - 35;
  int smokeY = y + 30;
  for (int i = 0; i < 4; i++) {
    MoveToEx(hdc, smokeX, smokeY + i * 3, NULL);
    LineTo(hdc, smokeX - 8 - i * 2, smokeY - 5 + i * 3);
    LineTo(hdc, smokeX - 15 - i * 2, smokeY - 8 + i * 3);
    LineTo(hdc, smokeX - 25 - i * 2, smokeY - 5 + i * 3);
  }

  // 4. Отдельные обломки вокруг
  HPEN debrisPen;
  if (f) debrisPen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));
  else debrisPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hdc, debrisPen);

  // Обломок 1
  MoveToEx(hdc, x + 40, y + 15, NULL);
  LineTo(hdc, x + 45, y + 18);
  LineTo(hdc, x + 43, y + 22);
  LineTo(hdc, x + 38, y + 20);
  LineTo(hdc, x + 40, y + 15);

  // Обломок 2
  MoveToEx(hdc, x - 45, y + 60, NULL);
  LineTo(hdc, x - 50, y + 65);
  LineTo(hdc, x - 48, y + 68);
  LineTo(hdc, x - 43, y + 66);
  LineTo(hdc, x - 45, y + 60);

  // 5. Капающее топливо
  HPEN fuelPen;
  if (f) fuelPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 0));
  else fuelPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hdc, fuelPen);

  MoveToEx(hdc, x - 20, y + 58, NULL);
  LineTo(hdc, x - 22, y + 65);
  MoveToEx(hdc, x - 22, y + 65, NULL);
  LineTo(hdc, x - 25, y + 72);
  MoveToEx(hdc, x - 25, y + 72, NULL);
  LineTo(hdc, x - 23, y + 78);

  DeleteObject(brokenPen);
  DeleteObject(crackPen);
  DeleteObject(missingPen);
  DeleteObject(smokePen);
  DeleteObject(debrisPen);
  DeleteObject(fuelPen);
}

void BrokenPlane::wings(bool f) {
    HPEN brokenPen;
  if (f) brokenPen = CreatePen(PS_DASH, 2, RGB(100, 100, 100));
  else brokenPen = CreatePen(PS_DASH, 2, RGB(0, 0, 0));
  SelectObject(hdc, brokenPen);

  // Поврежденные крылья
  MoveToEx(hdc, x + 12, y + 35, NULL);
  LineTo(hdc, x + 30, y + 45); // Короткое правое крыло

  MoveToEx(hdc, x - 8, y + 35, NULL);
  LineTo(hdc, x - 25, y + 40); // Сломанное левое крыло
  MoveToEx(hdc, x - 25, y + 40, NULL);
  LineTo(hdc, x - 35, y + 30); // Отогнутый вниз конец

  DeleteObject(brokenPen);
}

void BrokenPlane::hvost(bool f) {
  HPEN brokenPen;
  if (f) brokenPen = CreatePen(PS_DASH, 2, RGB(100, 100, 100));
  else brokenPen = CreatePen(PS_DASH, 2, RGB(0, 0, 0));
  SelectObject(hdc, brokenPen);

  // Обломки левого крыла
  MoveToEx(hdc, x - 25, y + 40, NULL);
  LineTo(hdc, x - 40, y + 50);
  MoveToEx(hdc, x - 40, y + 50, NULL);
  LineTo(hdc, x - 45, y + 45);

  // Хвостовая часть с повреждениями
  MoveToEx(hdc, x + 30, y + 45, NULL);
  LineTo(hdc, x + 30, y + 55);
  MoveToEx(hdc, x - 25, y + 40, NULL);
  LineTo(hdc, x - 25, y + 50);

  MoveToEx(hdc, x + 30, y + 55, NULL);
  LineTo(hdc, x + 10, y + 55);
  MoveToEx(hdc, x - 25, y + 50, NULL);
  LineTo(hdc, x - 8, y + 50);

  // Поврежденное шасси
  MoveToEx(hdc, x + 10, y + 55, NULL);
  LineTo(hdc, x + 10, y + 65); // Сломанная стойка
  MoveToEx(hdc, x - 8, y + 50, NULL);
  LineTo(hdc, x - 8, y + 52); // Короткая сломанная стойка
  MoveToEx(hdc, x - 8, y + 52, NULL);
  LineTo(hdc, x - 15, y + 55); // Отогнутое колесо

  MoveToEx(hdc, x + 10, y + 65, NULL);
  LineTo(hdc, x + 18, y + 68);
  MoveToEx(hdc, x - 15, y + 55, NULL);
  LineTo(hdc, x - 20, y + 58);

  MoveToEx(hdc, x + 18, y + 68, NULL);
  LineTo(hdc, x + 18, y + 70);
  MoveToEx(hdc, x - 20, y + 58, NULL);
  LineTo(hdc, x - 20, y + 60);

  MoveToEx(hdc, x + 18, y + 70, NULL);
  LineTo(hdc, x - 20, y + 60); // Перекошенная нижняя линия

  DeleteObject(brokenPen);
}

BrokenPlane::BrokenPlane(int _x, int _y) : Plane(_x, _y) {
  setHeight(80);
  setWidth(120);
  setCollisionX(-70);
  setCollisionY(0);
}

BrokenPlane::~BrokenPlane() {}