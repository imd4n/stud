#ifndef TBROKENPLANE_H
#define TBROKENPLANE_H

#include "TPlane.h"

// Наследуемся от самолета, чтобы сделать волшебный самолет
// Он просто имеет другую картинку)
class BrokenPlane:public Plane {
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