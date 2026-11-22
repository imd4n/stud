#include "TMagicPlane.h"
#include <wingdi.h>

extern HDC hdc;

MagicPlane::MagicPlane(int _x, int _y) : Plane(_x, _y) {
  setHeight(175);
  setWidth(175);
  setCollisionX(-85);
  setCollisionY(-25);
}

MagicPlane::~MagicPlane() {}

void MagicPlane::show() {
  // Установка толстого пера для основного контура
  HPEN thickPen = CreatePen(PS_SOLID, 3, RGB(0, 100, 200));
  SelectObject(hdc, thickPen);

  // Коллизия
  //Rectangle(hdc, x-85, y-25, x+90, y+150);

  // Основной корпус самолета (синий)
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + 15, y + 15);
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x - 15, y + 15);

  MoveToEx(hdc, x + 15, y + 15, NULL);
  LineTo(hdc, x + 15, y + 45);
  MoveToEx(hdc, x - 15, y + 15, NULL);
  LineTo(hdc, x - 15, y + 45);

  // Волшебные крылья с изогнутой формой
  MoveToEx(hdc, x + 15, y + 45, NULL);
  LineTo(hdc, x + 50, y + 60);
  MoveToEx(hdc, x + 50, y + 60, NULL);
  LineTo(hdc, x + 80, y + 40); // Изгиб крыла

  MoveToEx(hdc, x - 15, y + 45, NULL);
  LineTo(hdc, x - 50, y + 60);
  MoveToEx(hdc, x - 50, y + 60, NULL);
  LineTo(hdc, x - 80, y + 40); // Изгиб крыла

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

  // ВОЛШЕБНЫЕ ЭЛЕМЕНТЫ:

  // 1. Магический шар в кабине
  HBRUSH magicBrush = CreateSolidBrush(RGB(255, 215, 0)); // Золотой
  SelectObject(hdc, magicBrush);
  Ellipse(hdc, x - 8, y + 5, x + 8, y + 21);
  DeleteObject(magicBrush);

  // 2. Светящиеся двигатели
  HPEN glowPen = CreatePen(PS_SOLID, 2, RGB(255, 100, 0));
  SelectObject(hdc, glowPen);

  // Левый двигатель
  //Ellipse(hdc, x - 85, y + 35, x - 75, y + 45);
  MoveToEx(hdc, x - 80, y + 45, NULL);
  LineTo(hdc, x - 80, y + 55);
  MoveToEx(hdc, x - 85, y + 48, NULL);
  LineTo(hdc, x - 75, y + 48);

  // Правый двигатель
  //Ellipse(hdc, x + 75, y + 35, x + 85, y + 45);
  MoveToEx(hdc, x + 80, y + 45, NULL);
  LineTo(hdc, x + 80, y + 55);
  MoveToEx(hdc, x + 75, y + 48, NULL);
  LineTo(hdc, x + 85, y + 48);

  // 3. Магический след за самолетом
  HPEN trailPen = CreatePen(PS_SOLID, 1, RGB(148, 0, 211)); // Фиолетовый
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
  HPEN starPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
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

  // Восстановление старого пера
  DeleteObject(thickPen);
  DeleteObject(glowPen);
  DeleteObject(trailPen);
  DeleteObject(starPen);
}

void MagicPlane::hide() {
  // Установка толстого пера для основного контура
  HPEN thickPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
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

  // Волшебные крылья с изогнутой формой
  MoveToEx(hdc, x + 15, y + 45, NULL);
  LineTo(hdc, x + 50, y + 60);
  MoveToEx(hdc, x + 50, y + 60, NULL);
  LineTo(hdc, x + 80, y + 40); // Изгиб крыла

  MoveToEx(hdc, x - 15, y + 45, NULL);
  LineTo(hdc, x - 50, y + 60);
  MoveToEx(hdc, x - 50, y + 60, NULL);
  LineTo(hdc, x - 80, y + 40); // Изгиб крыла

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

  // ВОЛШЕБНЫЕ ЭЛЕМЕНТЫ:

  // 1. Магический шар в кабине
  HBRUSH magicBrush = CreateSolidBrush(RGB(0, 0, 0)); // Золотой
  HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, magicBrush);
  Ellipse(hdc, x - 8, y + 5, x + 8, y + 21);
  SelectObject(hdc, oldBrush);
  DeleteObject(magicBrush);

  // 2. Светящиеся двигатели
  HPEN glowPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, glowPen);

  // Левый двигатель
  //Ellipse(hdc, x - 85, y + 35, x - 75, y + 45);
  MoveToEx(hdc, x - 80, y + 45, NULL);
  LineTo(hdc, x - 80, y + 55);
  MoveToEx(hdc, x - 85, y + 48, NULL);
  LineTo(hdc, x - 75, y + 48);

  // Правый двигатель
  //Ellipse(hdc, x + 75, y + 35, x + 85, y + 45);
  MoveToEx(hdc, x + 80, y + 45, NULL);
  LineTo(hdc, x + 80, y + 55);
  MoveToEx(hdc, x + 75, y + 48, NULL);
  LineTo(hdc, x + 85, y + 48);

  // 3. Магический след за самолетом
  HPEN trailPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // Фиолетовый
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
  HPEN starPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
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

  // Восстановление старого пера
  DeleteObject(thickPen);
  DeleteObject(glowPen);
  DeleteObject(trailPen);
  DeleteObject(starPen);
}