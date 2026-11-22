#ifndef TREPAIREDPLANE_H
#define TREPAIREDPLANE_H

#include "TBrokenPlane.h"
class RepairedPlane : public BrokenPlane {
public:
  virtual void fuselage(bool f) override;
  virtual void wings(bool f) override;
  virtual void hvost(bool f) override;

  RepairedPlane(int _x, int _y); // Конструктор класса, для себя определяет его местоположение
  virtual ~RepairedPlane() = default; // Деструктор класса
};

#endif