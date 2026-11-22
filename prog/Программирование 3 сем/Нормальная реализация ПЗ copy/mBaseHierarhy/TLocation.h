#ifndef TLOCATION_H
#define TLOCATION_H

class Location
{
// Поменять protected на private
//private:
protected: // Доступно всем потомкам
    // Координаты
    int x;
    int y;
public:
    Location(int InitX, int InitY); // Конструктор
    ~Location(); // Деструктор (очистка памяти)

    // Геттеры
    int getX();
    int getY();

    // Сеттеры
    void setX(int newX);
    void setY(int newY);
}; // class Location

#endif