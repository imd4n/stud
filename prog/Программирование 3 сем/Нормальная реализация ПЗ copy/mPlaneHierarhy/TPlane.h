#ifndef TPLANE_H
#define TPLANE_H

#include "TCollision.h"
#include "TPoint.h"

// Класс самолета, явялется интерфейсом, не подразумевает реализации
// Не определяет обьект, а лишь его базовые возможности для создания
// конкретных реализаций в других классах
class Plane:public Point {
private:
    Collision *collision; // Обьект коллизии
    double scale; // Переменная размера самолета

    // Специфические свойства самолета
    int Passengers;
    int Crew; // Экипаж
    int maxHeight;
    int Weight;
public:
    // Конструктор класса, для себя определяет его местоположение
    Plane(int _x, int _y, double _scale = 1);
    // Деструктор класса
    ~Plane();

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
    Collision * getCollision();
};

#endif