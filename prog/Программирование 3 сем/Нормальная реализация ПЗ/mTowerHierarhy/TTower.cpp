#include "TTower.h"
#include <wingdi.h>

extern HDC hdc;

Tower::Tower(int _x, int _y) : Point(_x, _y) {
  /* Создание коллизии */
  width = 100;
  height = 450;
  collisionX = -10;
  collisionY = -450;
}

Tower::~Tower() {}

void Tower::setWidth(int newWidth) { width = newWidth; }

void Tower::setHeight(int newHeight) { height = newHeight; }

void Tower::setCollisionX(int newX) { collisionX = newX; }

void Tower::setCollisionY(int newY) { collisionY = newY; }

void Tower::show() {
  // Задали видимость
  Visible = true;

  // Установка пера для контура башни
  HPEN towerPen = CreatePen(PS_SOLID, 2, RGB(70, 70, 90));
  SelectObject(hdc, towerPen);

  int towerWidth = 80;
  int towerHeight = 400;
  int baseHeight = 30;

  // Проверка коллизии
  // Rectangle(hdc, x-10, y-450, x+90, y);

  // Фундамент башни
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + towerWidth, y);
  LineTo(hdc, x + towerWidth, y - baseHeight);
  LineTo(hdc, x, y - baseHeight);
  LineTo(hdc, x, y);

  // Основная башня (сужающаяся кверху)
  int topWidth = towerWidth - 20;
  MoveToEx(hdc, x, y - baseHeight, NULL);
  LineTo(hdc, x + towerWidth, y - baseHeight);
  LineTo(hdc, x + towerWidth / 2 + topWidth / 2, y - towerHeight);
  LineTo(hdc, x + towerWidth / 2 - topWidth / 2, y - towerHeight);
  LineTo(hdc, x, y - baseHeight);

  // Вертикальные элементы конструкции
  for (int i = 1; i < 4; i++) {
    int segX = x + i * (towerWidth / 4);
    MoveToEx(hdc, segX, y - baseHeight, NULL);
    LineTo(hdc, segX, y - towerHeight + 50);
  }

  // Горизонтальные этажи
  for (int i = 1; i < 15; i++) {
    int floorY = y - baseHeight - i * (towerHeight - 50) / 15;
    MoveToEx(hdc, x, floorY, NULL);
    LineTo(hdc, x + towerWidth, floorY);
  }

  // Верхняя часть с шпилем
  HPEN spirePen = CreatePen(PS_SOLID, 1, RGB(50, 50, 70));
  SelectObject(hdc, spirePen);

  int spireHeight = 60;
  MoveToEx(hdc, x + towerWidth / 2 - 5, y - towerHeight, NULL);
  LineTo(hdc, x + towerWidth / 2, y - towerHeight - spireHeight);
  LineTo(hdc, x + towerWidth / 2 + 5, y - towerHeight);

  // Окна - вертикальные полосы
  HPEN windowPen = CreatePen(PS_SOLID, 1, RGB(200, 230, 255));
  SelectObject(hdc, windowPen);

  for (int col = 0; col < 3; col++) {
    for (int floor = 0; floor < 12; floor++) {
      int windowX = x + 15 + col * 20;
      int windowY = y - baseHeight - 40 - floor * 30;

      // Вертикальное окно
      MoveToEx(hdc, windowX, windowY, NULL);
      LineTo(hdc, windowX, windowY - 15);
      MoveToEx(hdc, windowX + 8, windowY, NULL);
      LineTo(hdc, windowX + 8, windowY - 15);

      // Горизонтальные перемычки в окнах
      MoveToEx(hdc, windowX, windowY - 5, NULL);
      LineTo(hdc, windowX + 8, windowY - 5);
      MoveToEx(hdc, windowX, windowY - 10, NULL);
      LineTo(hdc, windowX + 8, windowY - 10);
    }
  }

  // Декоративные элементы на фасаде
  HPEN decorPen = CreatePen(PS_SOLID, 1, RGB(180, 160, 140));
  SelectObject(hdc, decorPen);

  // Горизонтальные декоративные полосы
  for (int i = 0; i < 3; i++) {
    int decorY = y - baseHeight - 100 - i * 120;
    MoveToEx(hdc, x - 5, decorY, NULL);
    LineTo(hdc, x + towerWidth + 5, decorY);
    MoveToEx(hdc, x - 5, decorY + 3, NULL);
    LineTo(hdc, x + towerWidth + 5, decorY + 3);
  }

  // Очистка объектов GDI
  DeleteObject(towerPen);
  DeleteObject(spirePen);
  DeleteObject(windowPen);
  DeleteObject(decorPen);
}

void Tower::hide() {
  // Задали видимость
  Visible = false;

  // Установка пера для контура башни
  HPEN towerPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  SelectObject(hdc, towerPen);

  int towerWidth = 80;
  int towerHeight = 400;
  int baseHeight = 30;

  // Фундамент башни
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + towerWidth, y);
  LineTo(hdc, x + towerWidth, y - baseHeight);
  LineTo(hdc, x, y - baseHeight);
  LineTo(hdc, x, y);

  // Основная башня (сужающаяся кверху)
  int topWidth = towerWidth - 20;
  MoveToEx(hdc, x, y - baseHeight, NULL);
  LineTo(hdc, x + towerWidth, y - baseHeight);
  LineTo(hdc, x + towerWidth / 2 + topWidth / 2, y - towerHeight);
  LineTo(hdc, x + towerWidth / 2 - topWidth / 2, y - towerHeight);
  LineTo(hdc, x, y - baseHeight);

  // Вертикальные элементы конструкции
  for (int i = 1; i < 4; i++) {
    int segX = x + i * (towerWidth / 4);
    MoveToEx(hdc, segX, y - baseHeight, NULL);
    LineTo(hdc, segX, y - towerHeight + 50);
  }

  // Горизонтальные этажи
  for (int i = 1; i < 15; i++) {
    int floorY = y - baseHeight - i * (towerHeight - 50) / 15;
    MoveToEx(hdc, x, floorY, NULL);
    LineTo(hdc, x + towerWidth, floorY);
  }

  // Верхняя часть с шпилем
  HPEN spirePen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  SelectObject(hdc, spirePen);

  int spireHeight = 60;
  MoveToEx(hdc, x + towerWidth / 2 - 5, y - towerHeight, NULL);
  LineTo(hdc, x + towerWidth / 2, y - towerHeight - spireHeight);
  LineTo(hdc, x + towerWidth / 2 + 5, y - towerHeight);

  // Окна - вертикальные полосы
  HPEN windowPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  SelectObject(hdc, windowPen);

  for (int col = 0; col < 3; col++) {
    for (int floor = 0; floor < 12; floor++) {
      int windowX = x + 15 + col * 20;
      int windowY = y - baseHeight - 40 - floor * 30;

      // Вертикальное окно
      MoveToEx(hdc, windowX, windowY, NULL);
      LineTo(hdc, windowX, windowY - 15);
      MoveToEx(hdc, windowX + 8, windowY, NULL);
      LineTo(hdc, windowX + 8, windowY - 15);

      // Горизонтальные перемычки в окнах
      MoveToEx(hdc, windowX, windowY - 5, NULL);
      LineTo(hdc, windowX + 8, windowY - 5);
      MoveToEx(hdc, windowX, windowY - 10, NULL);
      LineTo(hdc, windowX + 8, windowY - 10);
    }
  }

  // Декоративные элементы на фасаде
  HPEN decorPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  SelectObject(hdc, decorPen);

  // Горизонтальные декоративные полосы
  for (int i = 0; i < 3; i++) {
    int decorY = y - baseHeight - 100 - i * 120;
    MoveToEx(hdc, x - 5, decorY, NULL);
    LineTo(hdc, x + towerWidth + 5, decorY);
    MoveToEx(hdc, x - 5, decorY + 3, NULL);
    LineTo(hdc, x + towerWidth + 5, decorY + 3);
  }

  // Очистка объектов GDI
  DeleteObject(towerPen);
  DeleteObject(spirePen);
  DeleteObject(windowPen);
  DeleteObject(decorPen);
}

int Tower::getHeight() { return height; }

int Tower::getWidth() { return width; }

int Tower::getCollisionX() { return y + collisionX; }

int Tower::getCollisionY() { return y + collisionY; }