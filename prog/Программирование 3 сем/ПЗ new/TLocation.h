#ifdef TLOCATION_H
#define TLOCATION_H

class Location
{
//private:
protected: // Доступно всем потомкам
    // Координаты
    int x;
    int y;
public:
    Location(int InitX, int InitY); // Конструктор
    ~Location(); // Деструктор (очистка памяти)

    // Получение
    int getX();
    int getY();
    // Изменение значений полей
    void setX(int newX);
    void setY(int newY);
}; // class Location

#endif
