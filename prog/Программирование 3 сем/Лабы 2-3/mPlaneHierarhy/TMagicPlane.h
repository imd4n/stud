#ifndef TMAGICPLANE_H
#define TMAGICPLANE_H

#include "TPlane.h"

// Наследуемся от самолета, чтобы сделать волшебный самолет
// Он просто имеет другую картинку)
class MagicPlane:public Plane {
private:
const int ID3 = 1;
public:
    // Конструктор класса, для себя определяет его местоположение
    MagicPlane(int _x, int _y);
    // Деструктор класса
    virtual ~MagicPlane();

    // Показать/скрыть обьект
    virtual void show() override;
    virtual void hide() override;

    // Сеттеры
    /* Представим, что они тут есть */

    // Геттеры
    int getWidth();
    int getHeight();
    int getID() const {
        return ID3;
    }
};

#endif