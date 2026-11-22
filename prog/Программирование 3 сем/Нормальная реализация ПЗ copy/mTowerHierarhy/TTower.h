#ifndef TPLANE_H
#define TPLANE_H

#include "TCollision.h"
#include "TPoint.h"

class Tower:public Point {
private:
    Collision *collision; // Обьект коллизии
    double scale; // Переменная размера башни 

    // Специфические свойства
public:
    // Конструктор класса, для себя определяет его местоположение
    Tower(int _x, int _y, double _scale = 1);
    // Деструктор класса
    ~Tower();

    

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
    Collision * getCollision();
};

#endif