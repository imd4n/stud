#ifndef TMAGICTOWER_H
#define TMAGICTOWER_H

#include "TTower.h"

class MagicTower : public Tower {
public:
  // Конструктор класса, для себя определяет его местоположение
  MagicTower(int _x, int _y);
  // Деструктор класса
  virtual ~MagicTower() = default;

  // Показать/скрыть обьект
  virtual void show() override;
  virtual void hide() override;
};

#endif