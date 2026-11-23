#ifndef MAGICPLANE_H
#define MAGICPLANE_H

#include "Plane.h"

// Магический самолёт на время бонуса: та же физика, другая картинка.
class MagicPlane : public Plane {
private:
public:
    virtual void fuselage(bool f) override;
    virtual void wings(bool f) override;
    virtual void hvost(bool f) override; 

    // Конструктор класса, для себя определяет его местоположение
    MagicPlane(int _x = 0, int _y = 0);
    // Деструктор класса
    virtual ~MagicPlane();

    // Показать/скрыть обьект

    // Сеттеры
    /* Представим, что они тут есть */
};

#endif