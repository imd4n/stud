#ifndef TROCKETS_H
#define TROCKETS_H

// Подключили библиотеку для создания потомка от нее
#include "TPoint.h"
#include <cmath>

// Потомок "точка"
class Rockets : public Point {
private:
  int width;      // Ширина обьекта
  int height;     // Высота обьекта
  int collisionX; // Точка начала коллизии относительно начальной точки
                  // отрисовки
  int collisionY;

protected:
  bool Visible = false;

  int centerX;
  int centerY;
  int radius = 20;

  double phi = 0;

public:
  Rockets(int _x, int _y); // Конструктор
  ~Rockets() = default;    // Деструктор

  // Функции для работы с точкой
  virtual void show(); // Показать точку
  virtual void hide(); // Скрыть точку

  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();

  virtual void moveTo(int _x, int _y) {
    hide();
    x = _x;
    y = _y;
    
    // ВАЖНО! Надо поменять координаты центра
    centerX = _x;
    centerY = _y;
    phi = 0;
    show();
  }


  virtual void move() {
    hide();
    centerY -= 10;
    y = centerY;
    x = centerX + radius*sin(phi);
    phi += 0.2;
    show();
  }

  bool getVisible() { return Visible; }
  void setVisible(bool f) { Visible = f; }
}; // class Point

#endif