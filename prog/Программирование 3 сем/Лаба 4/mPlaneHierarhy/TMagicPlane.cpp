#include "TMagicPlane.h"
#include <wingdi.h>

extern HDC hdc;

void MagicPlane::fuselage(bool f) {
  // Задали видимость
  Visible = true;

  HPEN thickPen;
  if (f)
    thickPen = CreatePen(PS_SOLID, 3, RGB(0, 100, 200));
  else
    thickPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
  SelectObject(hdc, thickPen);

  // Основной корпус самолета (синий)
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + 15, y + 15);
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x - 15, y + 15);
  MoveToEx(hdc, x + 15, y + 15, NULL);
  LineTo(hdc, x + 15, y + 45);
  MoveToEx(hdc, x - 15, y + 15, NULL);
  LineTo(hdc, x - 15, y + 45);

  // ВОЛШЕБНЫЕ ЭЛЕМЕНТЫ:

  // 1. Магический шар в кабине
  HBRUSH magicBrush;
  if (f)
    magicBrush = CreateSolidBrush(RGB(255, 215, 0)); // Золотой
  else
    magicBrush = CreateSolidBrush(RGB(0, 0, 0));
  SelectObject(hdc, magicBrush);
  Ellipse(hdc, x - 8, y + 5, x + 8, y + 21);
  DeleteObject(magicBrush);

  // 2. Светящиеся двигатели
  HPEN glowPen;
  if (f)
    glowPen = CreatePen(PS_SOLID, 2, RGB(255, 100, 0));
  else
    glowPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, glowPen);

  // Левый двигатель
  // Ellipse(hdc, x - 85, y + 35, x - 75, y + 45);
  MoveToEx(hdc, x - 80, y + 45, NULL);
  LineTo(hdc, x - 80, y + 55);
  MoveToEx(hdc, x - 85, y + 48, NULL);
  LineTo(hdc, x - 75, y + 48);

  // Правый двигатель
  // Ellipse(hdc, x + 75, y + 35, x + 85, y + 45);
  MoveToEx(hdc, x + 80, y + 45, NULL);
  LineTo(hdc, x + 80, y + 55);
  MoveToEx(hdc, x + 75, y + 48, NULL);
  LineTo(hdc, x + 85, y + 48);

  // 3. Магический след за самолетом
  HPEN trailPen;
  if (f)
    trailPen = CreatePen(PS_SOLID, 1, RGB(148, 0, 211)); // Фиолетовый
  else
    trailPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hdc, trailPen);

  for (int i = 0; i < 5; i++) {
    MoveToEx(hdc, x - 25 + i * 2, y + 98, NULL);
    LineTo(hdc, x - 40 - i * 8, y + 120 + i * 5);
  }

  for (int i = 0; i < 5; i++) {
    MoveToEx(hdc, x + 25 - i * 2, y + 98, NULL);
    LineTo(hdc, x + 40 + i * 8, y + 120 + i * 5);
  }

  // 4. Звезды вокруг самолета
  HPEN starPen;
  if (f)
    starPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
  else
    starPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hdc, starPen);

  // Несколько маленьких звезд
  POINT stars[] = {{x + 60, y - 10},
                   {x - 70, y + 20},
                   {x + 90, y + 15},
                   {x - 40, y - 5},
                   {x + 30, y - 20}};

  for (int i = 0; i < 5; i++) {
    // Простые крестики для звезд
    MoveToEx(hdc, stars[i].x - 2, stars[i].y, NULL);
    LineTo(hdc, stars[i].x + 2, stars[i].y);
    MoveToEx(hdc, stars[i].x, stars[i].y - 2, NULL);
    LineTo(hdc, stars[i].x, stars[i].y + 2);
  }
}

void MagicPlane::wings(bool f) {
  HPEN thickPen;
  if (f)
    thickPen = CreatePen(PS_SOLID, 3, RGB(0, 100, 200));
  else
    thickPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
  SelectObject(hdc, thickPen);

  // Волшебные крылья с изогнутой формой
  MoveToEx(hdc, x + 15, y + 45, NULL);
  LineTo(hdc, x + 50, y + 60);
  MoveToEx(hdc, x + 50, y + 60, NULL);
  LineTo(hdc, x + 80, y + 40); // Изгиб крыла

  MoveToEx(hdc, x - 15, y + 45, NULL);
  LineTo(hdc, x - 50, y + 60);
  MoveToEx(hdc, x - 50, y + 60, NULL);
  LineTo(hdc, x - 80, y + 40); // Изгиб крыла
}

void MagicPlane::hvost(bool f) {
  HPEN thickPen;
  if (f)
    thickPen = CreatePen(PS_SOLID, 3, RGB(0, 100, 200));
  else
    thickPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
  SelectObject(hdc, thickPen);

  // Магический хвост
  MoveToEx(hdc, x + 50, y + 60, NULL);
  LineTo(hdc, x + 50, y + 70);
  MoveToEx(hdc, x - 50, y + 60, NULL);
  LineTo(hdc, x - 50, y + 70);

  MoveToEx(hdc, x + 50, y + 70, NULL);
  LineTo(hdc, x + 15, y + 70);
  MoveToEx(hdc, x - 50, y + 70, NULL);
  LineTo(hdc, x - 15, y + 70);

  // Шасси
  MoveToEx(hdc, x + 15, y + 70, NULL);
  LineTo(hdc, x + 15, y + 90);
  MoveToEx(hdc, x - 15, y + 70, NULL);
  LineTo(hdc, x - 15, y + 90);

  MoveToEx(hdc, x + 15, y + 90, NULL);
  LineTo(hdc, x + 25, y + 95);
  MoveToEx(hdc, x - 15, y + 90, NULL);
  LineTo(hdc, x - 25, y + 95);

  MoveToEx(hdc, x + 25, y + 95, NULL);
  LineTo(hdc, x + 25, y + 98);
  MoveToEx(hdc, x - 25, y + 95, NULL);
  LineTo(hdc, x - 25, y + 98);

  MoveToEx(hdc, x + 25, y + 98, NULL);
  LineTo(hdc, x - 25, y + 98);
}

MagicPlane::MagicPlane(int _x, int _y) : Plane(_x, _y) {
  setHeight(175);
  setWidth(175);
  setCollisionX(-85);
  setCollisionY(-25);

  ID = 2;
}