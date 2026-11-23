#ifndef POINT_H
#define POINT_H

// Подключили библиотеку для создания потомка от нее
#include "Location.h"
#include <winsock.h>

// Потомок "точка"
class Point : public Location {
public:
  Point(int _x, int _y); // Конструктор
  ~Point();              // Деструктор

  // Функции для работы с точкой
  virtual void show() = 0; // Показать точку
  virtual void hide() = 0; // Скрыть точку

  void moveTo(int newX, int newY); // Перемещение
}; // class Point

#endif