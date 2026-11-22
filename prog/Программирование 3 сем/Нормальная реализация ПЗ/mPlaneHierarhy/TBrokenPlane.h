#ifndef TBROKENPLANE_H
#define TBROKENPLANE_H

#include "TPlane.h"

// Наследуемся от самолета, чтобы сделать волшебный самолет
// Он просто имеет другую картинку)
class BrokenPlane:public Plane {
private:

public:
    // Конструктор класса, для себя определяет его местоположение
    BrokenPlane(int _x, int _y);
    // Деструктор класса
    ~BrokenPlane();

    // Показать/скрыть обьект
    virtual void show() override;
    virtual void hide() override;

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
    int getWidth();
    int getHeight();
};

#endif