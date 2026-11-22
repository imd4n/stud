#include "TPlane.h"
#include <wingdi.h>

extern HDC hdc;

void Plane::fuselage(bool f) {
  // Задали видимость
  Visible = true;

  HPEN Pen;
  if (f)
    Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
  else
    Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
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

  MoveToEx(hdc, x + 10, y + 75, NULL);
  LineTo(hdc, x + 10, y + 95);

  MoveToEx(hdc, x - 10, y + 75, NULL);
  LineTo(hdc, x - 10, y + 95);

  DeleteObject(Pen);
}

void Plane::wings(bool f) {
  HPEN Pen;
  if (f)
    Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
  else
    Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, Pen);

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
}

void Plane::hvost(bool f) {
  HPEN Pen;
  if (f)
    Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
  else
    Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, Pen);

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
}

void Plane::rockets(bool f) {}

Plane::Plane(int _x, int _y) : Point(_x, _y) {
  /* Создание коллизии */
  setWidth(90);
  setHeight(103);
  setCollisionX(-45);
  setCollisionY(0);
}

void Plane::setWidth(int newWidth) { width = newWidth; }

void Plane::setHeight(int newHeight) { height = newHeight; }

void Plane::setCollisionX(int newX) { collisionX = newX; }

void Plane::setCollisionY(int newY) { collisionY = newY; }

int Plane::getHeight() { return height; }

int Plane::getWidth() { return width; }

int Plane::getCollisionX() { return x + collisionX; }

int Plane::getCollisionY() { return y + collisionY; }

void Plane::show() {
  // Единичка значит, что надо показать обьект
  fuselage(1);
  wings(1);
  hvost(1);
  rockets(1);
}

void Plane::hide() {
  // Ноль значит — спрятать
  fuselage(0);
  wings(0);
  hvost(0);
  rockets(0);
}

// Отрисовка рокет
void AttackPlane::rockets(bool f) {
  MoveToEx(hdc, x + 20, y + 40, NULL);
  LineTo(hdc, x + 20, y + 80);

  MoveToEx(hdc, x - 20, y + 40, NULL);
  LineTo(hdc, x - 20, y + 80);
}

// Ну раз просили, вот атакующий самолет с ракетами
AttackPlane::AttackPlane(int _x, int _y) : Plane(_x, _y) {
  /* Создание коллизии */
  setWidth(90);
  setHeight(103);
  setCollisionX(-45);
  setCollisionY(0);

  ID = 1; // Установка уникального идентификатора
}