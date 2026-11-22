#ifndef TREPAIREDPLANE_H
#define TREPAIREDPLANE_H

#include "TBrokenPlane.h"
class RepairedPlane:public BrokenPlane {
private:
const int ID2 = 3;
public:
    // Конструктор класса, для себя определяет его местоположение
    RepairedPlane(int _x, int _y);
    // Деструктор класса
    virtual ~RepairedPlane();

    // Показать/скрыть обьект
    virtual void show() override;
    virtual void hide() override;

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
    int getWidth();
    int getHeight();
    int getID() const {
        return ID2;
    }
};

#endif