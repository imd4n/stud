#ifndef TBROKENPLANE_H
#define TBROKENPLANE_H

#include "TPlane.h"

// Наследуемся от самолета, чтобы сделать волшебный самолет
// Он просто имеет другую картинку)
class BrokenPlane:public Plane {
private:
  const int ID1 = 2;

public:
    // Конструктор класса, для себя определяет его местоположение
    BrokenPlane(int _x, int _y);
    // Деструктор класса
    virtual ~BrokenPlane();

    // Показать/скрыть обьект
    virtual void show() override;
    virtual void hide() override;

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
    int getWidth();
    int getHeight();
    int getID() const {
        return ID1;
    }
};

#endif