#include "TLocation.h"

// Конструктор
Location::Location(int _x, int _y) {
  x = _x;
  y = _y;
}

// Деструктор
Location::~Location() {}

// Геттеры
int Location::getX() { return x; }
int Location::getY() { return y; }

// Сеттеры
void Location::setX(int newX) { x = newX; }
void Location::setY(int newY) { y = newY; }