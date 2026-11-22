#ifndef TROCKETSFILL_H
#define TROCKETSFILL_H

// Подключили библиотеку для создания потомка от нее
#include "TPoint.h"

// Потомок "точка"
class RocketsFill : public Point {
private:
  int width;      // Ширина обьекта
  int height;     // Высота обьекта
  int collisionX; // Точка начала коллизии относительно начальной точки
                  // отрисовки
  int collisionY;

public:
  RocketsFill(int _x, int _y); // Конструктор
  ~RocketsFill() = default;    // Деструктор

  // Функции для работы с точкой
  virtual void show(); // Показать точку
  virtual void hide(); // Скрыть точку

  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();
}; // class Point

#endif