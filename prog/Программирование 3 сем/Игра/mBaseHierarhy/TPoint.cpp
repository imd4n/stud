#include "TPoint.h"
// Реализация методов в классе point
#include <windows.h>
extern HDC hdc;

// Конструктор, используем конструктор предка, но добавляем обработку нового
// поля
Point::Point(int _x, int _y) : Location(_x, _y) {}

// Деструктор
Point::~Point() {}

// Переместить обьект
void Point::moveTo(int newX, int newY) {
  hide();     // Скрыли старую точку
  setX(newX); // Изменили координату Х
  setY(newY); // Изменили координату У
  show();     // Показали новую точку
}