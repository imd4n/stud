#ifndef TCOLLISION_H
#define TCOLLISION_H

#include "TPoint.h"

// Класс обьекта определения коллизии
// Не имеет отрисовки, необходима только для определения коллизии.
// Является конечным обьектом, не подразумевает последующую иерархию
class Collision:public Point {
private:
    int width;  // Ширина
    int height; // Высота
public:
    // Конструктор и деструктор
    Collision(int _x, int _y, int _width, int _height);
    ~Collision();

    // Геттеры
    int getWidth();
    int getHeight();
    
    // Отрисовка
    //virtual void drawObj(u_char r, u_char g, u_char b) override;

    // Функция определения пересечения с другим обьектом коллизии
    bool checkCollision(Collision* col);
};

#endif