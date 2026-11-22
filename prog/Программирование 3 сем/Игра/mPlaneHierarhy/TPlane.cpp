#include "TPlane.h"
#include <wingdi.h>

extern HDC hdc;

void Plane::fuselage(bool f) {
  HPEN Pen;
  if (f)
    Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
  else
    Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  SelectObject(hdc, Pen);

  // Отрисовка
  //
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

Plane::Plane(int _x, int _y) : Point(_x, _y) {
  /* Создание коллизии */
  setWidth(90);
  setHeight(103);
  setCollisionX(-45);
  setCollisionY(0);

  ID = 0;
}

Plane::~Plane() {}

void Plane::setWidth(int newWidth) { width = newWidth; }

void Plane::setHeight(int newHeight) { height = newHeight; }

void Plane::setCollisionX(int newX) { collisionX = newX; }

void Plane::setCollisionY(int newY) { collisionY = newY; }

int Plane::getHeight() { return height; }

int Plane::getWidth() { return width; }

int Plane::getCollisionX() { return x + collisionX; }

int Plane::getCollisionY() { return y + collisionY; }

void Plane::show() {
  fuselage(1);
  wings(1);
  hvost(1);
}

void Plane::hide() {
  fuselage(0);
  wings(0);
  hvost(0);
  rocket1(0);
  rocket2(0);
}

void AttackPlane::rocket1(bool f) {
  if (f) {
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Rectangle(hdc, x + 20, y + 40, x + 30, y + 80);
    DeleteObject(pen);
    DeleteObject(brush);
  }
}

void AttackPlane::rocket2(bool f) {
  if (f) {
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Rectangle(hdc, x - 20, y + 40, x - 30, y + 80);
    DeleteObject(pen);
    DeleteObject(brush);
  }
}

AttackPlane::AttackPlane(int _x, int _y) : Plane(_x, _y) {
  r[0] = new Rockets(0, 0);
  r[1] = new Rockets(0, 0);
  /* Создание коллизии */
  setWidth(90);
  setHeight(103);
  setCollisionX(-45);
  setCollisionY(0);

  ID = 1;
}

AttackPlane::~AttackPlane() {
  delete r[0];
  delete r[1];
}

void AttackPlane::show() {
  fuselage(1);
  wings(1);
  hvost(1);

  // Рисуем каждую ракету только если она ещё не выпущена
  // Рисуем ракету 1, если она не видима И есть запас (rocketnum > 0)
  if (!r[0]->getVisible() && rocketnum > 0) {
    rocket1(true);
  }
  // Рисуем ракету 2, если она не видима И запас позволяет (rocketnum > 1)
  if (!r[1]->getVisible() && rocketnum > 1) {
    rocket2(true);
  }
}

// Метод setRockets
void AttackPlane::setRockets(int inc) {
  if (rocketnum < 2) {
    rocketnum += inc;
    if (rocketnum > 2)
      rocketnum = 2;
    for (int i = 0; i < 2; i++) {
      if (r[i]->getVisible()) {
        r[i]->hide();
        r[i]->setVisible(false);
        r[i]->moveTo(x, y + 40);
      }
    }
  }
}