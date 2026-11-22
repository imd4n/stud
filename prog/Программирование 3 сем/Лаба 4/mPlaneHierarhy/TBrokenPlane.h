#ifndef TBROKENPLANE_H
#define TBROKENPLANE_H

#include "TPlane.h"

// Наследуемся от самолета, чтобы сделать волшебный самолет
// Он просто имеет другую картинку)
class BrokenPlane : public Plane {
public:
  virtual void fuselage(bool f) override;
  virtual void wings(bool f) override;
  virtual void hvost(bool f) override;

  // Конструктор класса, для себя определяет его местоположение
  BrokenPlane(int _x, int _y);
  // Деструктор класса
  virtual ~BrokenPlane() = default;
};

#endif