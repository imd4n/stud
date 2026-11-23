#include "TRepairZone.h"
#include "TPoint.h"
#include <wingdi.h>

extern HDC hdc;

RepairZone::RepairZone(int _x, int _y) : Point(_x, _y) {
  // Установили размеры
  collisionX = 0;
  collisionY = 0;
  height = 100;
  width = 100;
}

void RepairZone::show() {
  // Белый фон
  HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
  HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
  SelectObject(hdc, pen);
  SelectObject(hdc, brush);
  Rectangle(hdc, x, y, x + width, y + height);
  DeleteObject(pen);
  DeleteObject(brush);

  // Красный крест (символ медицинской помощи)
  HPEN crossPen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
  SelectObject(hdc, crossPen);
  MoveToEx(hdc, x + 20, y + 20, NULL);
  LineTo(hdc, x + width - 20, y + height - 20);
  MoveToEx(hdc, x + width - 20, y + 20, NULL);
  LineTo(hdc, x + 20, y + height - 20);
  DeleteObject(crossPen);
}

void RepairZone::hide() {
  // Закрашиваем чёрным
  HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
  RECT rect = {x, y, x + width, y + height};
  FillRect(hdc, &rect, blackBrush);
  DeleteObject(blackBrush);
}

int RepairZone::getWidth() { return width; }
int RepairZone::getHeight() { return height; }
int RepairZone::getCollisionX() { return x + collisionX; }
int RepairZone::getCollisionY() { return y + collisionY; }