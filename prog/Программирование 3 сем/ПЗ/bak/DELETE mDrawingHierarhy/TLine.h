#ifndef TLINE_H
#define TLINE_H

#include "TPoint.h"

class Line:public Point
{
private:
    double angle;
    double length;
public:
    Line(int initX1, int initY1, int initX2, int initY2); // Конструктор
    Line(int initX, int initY, double _angle, double _length);
    ~Line(); // Деструктор

    // Функции для работы с точкой
    void Show(); // Показать
    void Hide(); // Спрятать

    void moveTo(int newX, int newY); // Перемещение

    // TODO: Сделать изменение угла, длинны, а также перемещение по двум точкам
    // Дописать сет и гет
}; // class Point

#endif