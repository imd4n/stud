#ifndef LOCATION_H
#define LOCATION_H

class Location
{
protected: // Доступно всем потомкам
    // Координаты
    int x;
    int y;
public:
    Location(int _x, int _y); // Конструктор
    virtual ~Location(); // Деструктор (очистка памяти)

    // Геттеры
    int getX();
    int getY();

    // Сеттеры
    void setX(int newX);
    void setY(int newY);
}; // class Location

#endif