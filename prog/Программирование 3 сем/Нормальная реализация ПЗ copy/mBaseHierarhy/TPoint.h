#ifndef TPOINT_H
#define TPOINT_H

// Подключили библиотеку для создания потомка от нее
#include "TLocation.h"
#include <winsock.h>

// Потомок "точка"
class Point:public Location
{
protected:
    // Задаем только одну переменную, так как остальные наследуются
    bool Visible;
    // Матрица, которая послужит для хранения элемента.
    u_char * bitMap; 
    int mapN, mapM; // Размеры матрицы
public:
    Point (int _x, int _y, int _mapN, int _mapM); // Конструктор
    ~Point(); // Деструктор

    // Функция отрисовки обьекта (чисто виртуальная)
    void drawObj(u_char r, u_char g, u_char b);

    // Функции для работы с точкой
    void show(); // Показать точку
    void hide(); // Скрыть точку
    bool getVisible(); // Получение состояния

    void moveTo(int newX, int newY); // Перемещение

}; // class Point

#endif