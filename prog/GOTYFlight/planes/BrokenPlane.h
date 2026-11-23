#ifndef BROKENPLANE_H
#define BROKENPLANE_H

#include "Plane.h"

// Сломанный самолёт: показывается после столкновения.
class BrokenPlane : public Plane {
private:
public:
    virtual void fuselage(bool f) override;
    virtual void wings(bool f) override;
    virtual void hvost(bool f) override; 


    // Конструктор класса, для себя определяет его местоположение
    BrokenPlane(int _x = 0, int _y = 0);
    // Деструктор класса
    virtual ~BrokenPlane();

    // Показать/скрыть обьект

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
    //int getWidth(); 
    //int getHeight();
};

#endif