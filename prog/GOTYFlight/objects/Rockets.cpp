#include "Rockets.h"

extern HDC hdc;

Rockets::Rockets(int _x, int _y) : Point(_x, _y) {
  collisionX = 0;
  collisionY = 0;
  width = 10;
  height = 20;
}

void Rockets::show() {
  HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
  RECT rect = {x, y, x + width, y + height};
  FillRect(hdc, &rect, brush);
  DeleteObject(brush);
}

void Rockets::hide() {
  HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
  RECT rect = {x, y, x + width, y + height};
  FillRect(hdc, &rect, brush);
  DeleteObject(brush);
}

int Rockets::getWidth() { return width; }
int Rockets::getHeight() { return height; }
int Rockets::getCollisionX() { return x + collisionX; }
int Rockets::getCollisionY() { return y + collisionY; }
