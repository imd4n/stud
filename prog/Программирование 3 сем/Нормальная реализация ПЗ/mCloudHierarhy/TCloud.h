#ifndef TCLOUD_H
#define TCLOUD_H

#include "TCollision.h"
#include "TPoint.h"

class Cloud:public Point {
private:
    // Поля для определения коллизии
    int width;  // Ширина
    int height; // Высота

    double scale; // Переменная размера

public:
    // Конструктор класса, для себя определяет его местоположение
    Cloud(int _x, int _y, double _scale = 1);
    // Деструктор класса
    ~Cloud();

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
    Collision * getCollision();
};

#endif