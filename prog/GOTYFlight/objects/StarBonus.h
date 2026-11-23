#include <windows.h>
#include "Point.h"

extern HDC hdc;

// Класс звёздочки (бонус)
class StarBonus : public Point {
private:
  int width = 30;
  int height = 30;

public:
  StarBonus(int x, int y) : Point(x, y) {}
  void show() override {
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    POINT pts[4] = {{x, y - 15}, {x + 15, y}, {x, y + 15}, {x - 15, y}};
    Polygon(hdc, pts, 4);
    DeleteObject(pen);
    DeleteObject(brush);
  }
  void hide() override {
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    POINT pts[4] = {{x, y - 15}, {x + 15, y}, {x, y + 15}, {x - 15, y}};
    Polygon(hdc, pts, 4);
    DeleteObject(pen);
    DeleteObject(brush);
  }
  void moveDown() {
    hide();
    y += 3;
    show();
  }

  // Методы для коллизии
  int getCollisionX() { return x; }
  int getCollisionY() { return y; }
  int getWidth() { return width; }
  int getHeight() { return height; }
};
