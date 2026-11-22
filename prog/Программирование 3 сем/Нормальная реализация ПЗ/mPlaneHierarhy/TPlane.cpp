#include "TPlane.h"
#include <wingdi.h>

extern HDC hdc;

Plane::Plane(int _x, int _y) : Point(_x, _y) {
  /* Создание коллизии */
  width = 90;
  height = 103;
  collisionX = -45;
  collisionY = 0;
}

Plane::~Plane() {}

void Plane::setWidth(int newWidth) {
  width = newWidth;
}

void Plane::setHeight(int newHeight) {
  height = newHeight;
}

void Plane::setCollisionX(int newX) {
  collisionX = newX;
}

void Plane::setCollisionY(int newY) {
  collisionY = newY;
}

void Plane::show() {
  // Задали видимость
  Visible = true;

  HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
  SelectObject(hdc, Pen);

  // Отрисовка
  
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + 10, y + 10);

  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x - 10, y + 10);

  MoveToEx(hdc, x + 10, y + 10, NULL);
  LineTo(hdc, x + 10, y + 50);

  MoveToEx(hdc, x - 10, y + 10, NULL);
  LineTo(hdc, x - 10, y + 50);

  MoveToEx(hdc, x + 10, y + 50, NULL);
  LineTo(hdc, x + 45, y + 65);

  MoveToEx(hdc, x - 10, y + 50, NULL);
  LineTo(hdc, x - 45, y + 65);

  MoveToEx(hdc, x + 45, y + 65, NULL);
  LineTo(hdc, x + 45, y + 75);

  MoveToEx(hdc, x - 45, y + 65, NULL);
  LineTo(hdc, x - 45, y + 75);

  MoveToEx(hdc, x + 45, y + 75, NULL);
  LineTo(hdc, x + 10, y + 75);

  MoveToEx(hdc, x - 45, y + 75, NULL);
  LineTo(hdc, x - 10, y + 75);

  MoveToEx(hdc, x + 10, y + 75, NULL);
  LineTo(hdc, x + 10, y + 95);

  MoveToEx(hdc, x - 10, y + 75, NULL);
  LineTo(hdc, x - 10, y + 95);

  MoveToEx(hdc, x + 10, y + 95, NULL);
  LineTo(hdc, x + 20, y + 100);

  MoveToEx(hdc, x - 10, y + 95, NULL);
  LineTo(hdc, x - 20, y + 100);

  MoveToEx(hdc, x + 20, y + 100, NULL);
  LineTo(hdc, x + 20, y + 103);

  MoveToEx(hdc, x - 20, y + 100, NULL);
  LineTo(hdc, x - 20, y + 103);

  MoveToEx(hdc, x + 20, y + 103, NULL);
  LineTo(hdc, x - 20, y + 103);
  DeleteObject(Pen);
}

void Plane::hide() {
  // Задали видимость
  Visible = true;

  HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, Pen);

  // Отрисовка
  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x + 10, y + 10);

  MoveToEx(hdc, x, y, NULL);
  LineTo(hdc, x - 10, y + 10);

  MoveToEx(hdc, x + 10, y + 10, NULL);
  LineTo(hdc, x + 10, y + 50);

  MoveToEx(hdc, x - 10, y + 10, NULL);
  LineTo(hdc, x - 10, y + 50);

  MoveToEx(hdc, x + 10, y + 50, NULL);
  LineTo(hdc, x + 45, y + 65);

  MoveToEx(hdc, x - 10, y + 50, NULL);
  LineTo(hdc, x - 45, y + 65);

  MoveToEx(hdc, x + 45, y + 65, NULL);
  LineTo(hdc, x + 45, y + 75);

  MoveToEx(hdc, x - 45, y + 65, NULL);
  LineTo(hdc, x - 45, y + 75);

  MoveToEx(hdc, x + 45, y + 75, NULL);
  LineTo(hdc, x + 10, y + 75);

  MoveToEx(hdc, x - 45, y + 75, NULL);
  LineTo(hdc, x - 10, y + 75);

  MoveToEx(hdc, x + 10, y + 75, NULL);
  LineTo(hdc, x + 10, y + 95);

  MoveToEx(hdc, x - 10, y + 75, NULL);
  LineTo(hdc, x - 10, y + 95);

  MoveToEx(hdc, x + 10, y + 95, NULL);
  LineTo(hdc, x + 20, y + 100);

  MoveToEx(hdc, x - 10, y + 95, NULL);
  LineTo(hdc, x - 20, y + 100);

  MoveToEx(hdc, x + 20, y + 100, NULL);
  LineTo(hdc, x + 20, y + 103);

  MoveToEx(hdc, x - 20, y + 100, NULL);
  LineTo(hdc, x - 20, y + 103);

  MoveToEx(hdc, x + 20, y + 103, NULL);
  LineTo(hdc, x - 20, y + 103);

  DeleteObject(Pen);
}

// Без виртуальных методов
void Plane::moveTo(int newX, int newY) {
  hide();
  setX(newX);
  setY(newY);
  show();
}

int Plane::getHeight() {
  return height;
}

int Plane::getWidth() {
  return width;
}

int Plane::getCollisionX() {
  return x+collisionX;
}

int Plane::getCollisionY() {
  return y+collisionY;
}