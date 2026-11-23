#include "MagicTower.h"
#include <wingdi.h>

extern HDC hdc;

MagicTower::MagicTower(int _x, int _y) : Tower(_x, _y) {
  /* Создание коллизии */
  setWidth(100);
  setHeight(450);
  setCollisionX(-10);
  setCollisionY(-450);

  ID = 1;
}

MagicTower::~MagicTower() {}

void MagicTower::show() {
  // Установка пера для контура башни - градиент от фиолетового к синему
  HPEN towerPen = CreatePen(PS_SOLID, 3, RGB(100, 60, 180));
  HPEN oldPen = (HPEN)SelectObject(hdc, towerPen);

  int towerWidth = 100;
  int towerHeight = 450;
  int baseHeight = 40;

  // Фундамент башни
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + towerWidth, y);
  LineTo(hdc, x + towerWidth, y - baseHeight);
  LineTo(hdc, x, y - baseHeight);
  LineTo(hdc, x, y);

  // Основная башня (изогнутая, волшебная форма)
  int topWidth = towerWidth - 30;
  POINT towerPoints[] = {{x, y - baseHeight},
                         {x + towerWidth, y - baseHeight},
                         {x + towerWidth + 10, y - towerHeight / 3},
                         {x + towerWidth - 5, y - towerHeight * 2 / 3},
                         {x + towerWidth / 2 + topWidth / 2, y - towerHeight},
                         {x + towerWidth / 2 - topWidth / 2, y - towerHeight},
                         {x + 5, y - towerHeight * 2 / 3},
                         {x - 10, y - towerHeight / 3},
                         {x, y - baseHeight}};
  Polyline(hdc, towerPoints, 9);

  // Верхняя часть с магическим кристаллом вместо шпиля
  HPEN crystalPen = CreatePen(PS_SOLID, 2, RGB(200, 180, 255));
  SelectObject(hdc, crystalPen);

  int crystalHeight = 80;
  POINT crystalPoints[] = {
      {x + towerWidth / 2 - 8, y - towerHeight},
      {x + towerWidth / 2, y - towerHeight - crystalHeight},
      {x + towerWidth / 2 + 8, y - towerHeight},
      {x + towerWidth / 2 - 8, y - towerHeight}};
  Polyline(hdc, crystalPoints, 4);

  // Внутренние грани кристалла
  MoveToEx(hdc, x + towerWidth / 2, y - towerHeight - crystalHeight, NULL);
  LineTo(hdc, x + towerWidth / 2 - 4, y - towerHeight - crystalHeight / 2);
  MoveToEx(hdc, x + towerWidth / 2, y - towerHeight - crystalHeight, NULL);
  LineTo(hdc, x + towerWidth / 2 + 4, y - towerHeight - crystalHeight / 2);

  // Окна
  HPEN windowPen = CreatePen(PS_SOLID, 1, RGB(180, 220, 255));
  SelectObject(hdc, windowPen);

  for (int col = 0; col < 3; col++) {
    for (int floor = 0; floor < 10; floor++) {
      int windowX = x + 20 + col * 25;
      int windowY = y - baseHeight - 60 - floor * 35;

      // Арочные окна
      MoveToEx(hdc, windowX, windowY, NULL);
      LineTo(hdc, windowX, windowY - 20);
      MoveToEx(hdc, windowX + 12, windowY, NULL);
      LineTo(hdc, windowX + 12, windowY - 20);

      // Арка над окном
      Arc(hdc, windowX - 2, windowY - 25, windowX + 14, windowY - 15,
          windowX + 14, windowY - 20, windowX - 2, windowY - 20);

      // Вертикальные разделители
      MoveToEx(hdc, windowX + 4, windowY, NULL);
      LineTo(hdc, windowX + 4, windowY - 20);
      MoveToEx(hdc, windowX + 8, windowY, NULL);
      LineTo(hdc, windowX + 8, windowY - 20);

      // Горизонтальные перемычки
      MoveToEx(hdc, windowX, windowY - 7, NULL);
      LineTo(hdc, windowX + 12, windowY - 7);
      MoveToEx(hdc, windowX, windowY - 14, NULL);
      LineTo(hdc, windowX + 12, windowY - 14);
    }
  }

  // Восстановление старого пера
  SelectObject(hdc, oldPen);

  // Очистка объектов GDI
  DeleteObject(towerPen);
  DeleteObject(crystalPen);
  DeleteObject(windowPen);
}

void MagicTower::hide() {
  // Установка пера для контура башни - градиент от фиолетового к синему
  HPEN towerPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
  HPEN oldPen = (HPEN)SelectObject(hdc, towerPen);

  int towerWidth = 100;
  int towerHeight = 450;
  int baseHeight = 40;

  // Фундамент башни
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + towerWidth, y);
  LineTo(hdc, x + towerWidth, y - baseHeight);
  LineTo(hdc, x, y - baseHeight);
  LineTo(hdc, x, y);

  // Основная башня (изогнутая, волшебная форма)
  int topWidth = towerWidth - 30;
  POINT towerPoints[] = {{x, y - baseHeight},
                         {x + towerWidth, y - baseHeight},
                         {x + towerWidth + 10, y - towerHeight / 3},
                         {x + towerWidth - 5, y - towerHeight * 2 / 3},
                         {x + towerWidth / 2 + topWidth / 2, y - towerHeight},
                         {x + towerWidth / 2 - topWidth / 2, y - towerHeight},
                         {x + 5, y - towerHeight * 2 / 3},
                         {x - 10, y - towerHeight / 3},
                         {x, y - baseHeight}};
  Polyline(hdc, towerPoints, 9);

  // Верхняя часть с магическим кристаллом вместо шпиля
  HPEN crystalPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, crystalPen);

  int crystalHeight = 80;
  POINT crystalPoints[] = {
      {x + towerWidth / 2 - 8, y - towerHeight},
      {x + towerWidth / 2, y - towerHeight - crystalHeight},
      {x + towerWidth / 2 + 8, y - towerHeight},
      {x + towerWidth / 2 - 8, y - towerHeight}};
  Polyline(hdc, crystalPoints, 4);

  // Внутренние грани кристалла
  MoveToEx(hdc, x + towerWidth / 2, y - towerHeight - crystalHeight, NULL);
  LineTo(hdc, x + towerWidth / 2 - 4, y - towerHeight - crystalHeight / 2);
  MoveToEx(hdc, x + towerWidth / 2, y - towerHeight - crystalHeight, NULL);
  LineTo(hdc, x + towerWidth / 2 + 4, y - towerHeight - crystalHeight / 2);

  // Окна
  HPEN windowPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  SelectObject(hdc, windowPen);

  for (int col = 0; col < 3; col++) {
    for (int floor = 0; floor < 10; floor++) {
      int windowX = x + 20 + col * 25;
      int windowY = y - baseHeight - 60 - floor * 35;

      // Арочные окна
      MoveToEx(hdc, windowX, windowY, NULL);
      LineTo(hdc, windowX, windowY - 20);
      MoveToEx(hdc, windowX + 12, windowY, NULL);
      LineTo(hdc, windowX + 12, windowY - 20);

      // Арка над окном
      Arc(hdc, windowX - 2, windowY - 25, windowX + 14, windowY - 15,
          windowX + 14, windowY - 20, windowX - 2, windowY - 20);

      // Вертикальные разделители
      MoveToEx(hdc, windowX + 4, windowY, NULL);
      LineTo(hdc, windowX + 4, windowY - 20);
      MoveToEx(hdc, windowX + 8, windowY, NULL);
      LineTo(hdc, windowX + 8, windowY - 20);

      // Горизонтальные перемычки
      MoveToEx(hdc, windowX, windowY - 7, NULL);
      LineTo(hdc, windowX + 12, windowY - 7);
      MoveToEx(hdc, windowX, windowY - 14, NULL);
      LineTo(hdc, windowX + 12, windowY - 14);
    }
  }

  // Восстановление старого пера
  SelectObject(hdc, oldPen);

  // Очистка объектов GDI
  DeleteObject(towerPen);
  DeleteObject(crystalPen);
  DeleteObject(windowPen);
}