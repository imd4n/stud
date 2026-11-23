#include "TRocketsFill.h"
#include "TPoint.h"
#include <wingdi.h>

extern HDC hdc;

RocketsFill::RocketsFill(int _x, int _y) : Point(_x, _y) {
  // Установили размеры
  collisionX = 0;
  collisionY = 0;
  height = 100;
  width = 100;
}

void RocketsFill::show() {
  // Рисуем ракету в центре зоны
  HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
  HBRUSH brush = CreateSolidBrush(RGB(255, 200, 0)); // оранжевый корпус
  SelectObject(hdc, pen);
  SelectObject(hdc, brush);

  // Корпус (вертикальный прямоугольник)
  Rectangle(hdc, x + 30, y + 30, x + 70, y + 80);

  // Нос (треугольник)
  POINT nose[3] = {{x + 30, y + 30}, {x + 70, y + 30}, {x + 50, y + 10}};
  Polygon(hdc, nose, 3);

  // Хвостовые стабилизаторы (два треугольника)
  POINT tail1[3] = {{x + 30, y + 80}, {x + 40, y + 80}, {x + 35, y + 95}};
  Polygon(hdc, tail1, 3);
  POINT tail2[3] = {{x + 60, y + 80}, {x + 70, y + 80}, {x + 65, y + 95}};
  Polygon(hdc, tail2, 3);
  DeleteObject(pen);
  DeleteObject(brush);

  // Окно (голубой круг)
  HPEN windowPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
  HBRUSH windowBrush = CreateSolidBrush(RGB(100, 100, 255));
  SelectObject(hdc, windowPen);
  SelectObject(hdc, windowBrush);
  Ellipse(hdc, x + 42, y + 40, x + 58, y + 56);
  DeleteObject(windowPen);
  DeleteObject(windowBrush);
}

void RocketsFill::hide() {
  // Закрашиваем чёрным
  HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
  RECT rect = {x, y, x + width, y + height};
  FillRect(hdc, &rect, blackBrush);
  DeleteObject(blackBrush);
}

int RocketsFill::getWidth() { return width; }
int RocketsFill::getHeight() { return height; }
int RocketsFill::getCollisionX() { return x + collisionX; }
int RocketsFill::getCollisionY() { return y + collisionY; }