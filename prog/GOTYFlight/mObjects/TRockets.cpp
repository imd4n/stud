#include "TRockets.h"

extern HDC hdc;

Rockets::Rockets(int _x, int _y) : Point(_x, _y) {
  collisionX = 0;
  collisionY = 0;
  height = 20;
  width = 10;

  centerX = _x;
  centerY = _y;
}

void Rockets::show() {
  Visible = true;
  HBRUSH background = CreateSolidBrush(RGB(255, 255, 255));
  RECT rect = {x, y, x + width, y + height};
  FillRect(hdc, &rect, background);
  DeleteObject(background);
}

void Rockets::hide() {
  Visible = false;
  HBRUSH background = CreateSolidBrush(RGB(0, 0, 0));
  RECT rect = {x, y, x + width, y + height};
  FillRect(hdc, &rect, background);
  DeleteObject(background);
}

int Rockets::getWidth() { return width; }
int Rockets::getHeight() { return height; }
int Rockets::getCollisionX() { return x + collisionX; }
int Rockets::getCollisionY() { return y + collisionY; }