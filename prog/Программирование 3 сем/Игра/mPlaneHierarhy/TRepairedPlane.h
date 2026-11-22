#ifndef TREPAIREDPLANE_H
#define TREPAIREDPLANE_H

#include "TBrokenPlane.h"
class RepairedPlane:public BrokenPlane {
private:
public:
virtual void fuselage(bool f) override;
    virtual void wings(bool f) override;
    virtual void hvost(bool f) override; 


    // Конструктор класса, для себя определяет его местоположение
    RepairedPlane(int _x = 0, int _y = 0);
    // Деструктор класса
    virtual ~RepairedPlane();

    // Показать/скрыть обьект

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
};

#endif