#ifndef TMAGICPLANE_H
#define TMAGICPLANE_H

#include "TPlane.h"

// Наследуемся от самолета, чтобы сделать волшебный самолет
// Он просто имеет другую картинку)
class MagicPlane:public AttackPlane {
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