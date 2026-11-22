#ifndef TPOINT_H
#define TPOINT_H

// Подключили библиотеку для создания потомка от нее
#include "TLocation.h"
#include <winsock.h>

// Потомок "точка"
class Point : public Location {
private:
protected:
  // Задаем только одну переменную, так как остальные наследуются
  bool Visible;

public:
  Point(int _x, int _y); // Конструктор
  ~Point();              // Деструктор

  // Функции для работы с точкой
  virtual void show(); // Показать точку
  virtual void hide(); // Скрыть точку

  // Реализация без виртуальности
  // void show();
  // void hide();

  bool getVisible(); // Получение состояния

  void moveTo(int newX, int newY); // Перемещение


}; // class Point

#endif