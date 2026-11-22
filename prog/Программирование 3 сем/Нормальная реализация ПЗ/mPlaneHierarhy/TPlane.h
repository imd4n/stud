#ifndef TPLANE_H
#define TPLANE_H

#include "TPoint.h"

// Класс самолета, явялется интерфейсом, не подразумевает реализации
// Не определяет обьект, а лишь его базовые возможности для создания
// конкретных реализаций в других классах
class Plane:public Point {
private:
    // Обеспечение возможности проверки коллизии
    int width; // Ширина обьекта
    int height; // Высота обьекта
    int collisionX; // Точка начала коллизии относительно начальной точки отрисовки
    int collisionY;

    // Специфические свойства самолета
protected:
    // Необходимо для потомков
    void setWidth(int newWidth);
    void setHeight(int newHeight);
    void setCollisionX(int newX);
    void setCollisionY(int newY);
public:
    // Конструктор класса, для себя определяет его местоположение
    Plane(int _x, int _y);
    // Деструктор класса
    ~Plane();

    // Показать/скрыть обьект
    //virtual void show() override;
    //virtual void hide() override;

    // Реализация без виртуальных методов
    void show();
    void hide();
    void moveTo(int newX, int newY);

    // Сеттеры

    // Геттеры
    int getWidth();
    int getHeight();
    int getCollisionX();
    int getCollisionY();
};

#endif