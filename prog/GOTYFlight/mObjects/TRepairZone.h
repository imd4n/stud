#ifndef TREPAIRZONE_H
#define TREPAIRZONE_H

// Подключили библиотеку для создания потомка от нее
#include "TPoint.h"

// Потомок "точка"
class RepairZone : public Point {
private:
  int width;      // Ширина обьекта
  int height;     // Высота обьекта
  int collisionX; // Точка начала коллизии относительно начальной точки
                  // отрисовки
  int collisionY;

public:
  RepairZone(int _x, int _y); // Конструктор
  ~RepairZone() = default;    // Деструктор

  // Функции для работы с точкой
  virtual void show(); // Показать точку
  virtual void hide(); // Скрыть точку

  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();
}; // class Point

#endif