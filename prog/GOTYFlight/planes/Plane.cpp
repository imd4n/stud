#include "Plane.h"
#include <wingdi.h>

extern HDC hdc;

void Plane::fuselage(bool f) {
  HPEN pen = CreatePen(PS_SOLID, 2,
                       f ? RGB(255, 255, 255) : RGB(0, 0, 0));
  SelectObject(hdc, pen);

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

  DeleteObject(pen);
}

void Plane::wings(bool f) {
  HPEN pen = CreatePen(PS_SOLID, 2,
                       f ? RGB(255, 255, 255) : RGB(0, 0, 0));
  SelectObject(hdc, pen);

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

  DeleteObject(pen);
}

void Plane::hvost(bool f) {
  HPEN pen = CreatePen(PS_SOLID, 2,
                       f ? RGB(255, 255, 255) : RGB(0, 0, 0));
  SelectObject(hdc, pen);

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

  DeleteObject(pen);
}

Plane::Plane(int _x, int _y) : Point(_x, _y) {
  setWidth(90);
  setHeight(103);
  setCollisionX(-45);
  setCollisionY(0);
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
  fuselage(true);
  wings(true);
  hvost(true);
}

void Plane::hide() {
  fuselage(false);
  wings(false);
  hvost(false);
}
