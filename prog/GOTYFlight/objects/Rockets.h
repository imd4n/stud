#ifndef ROCKETS_H
#define ROCKETS_H

#include "Point.h"

// Вражеская ракета: небольшой прямоугольник, летит справа налево.
class Rockets : public Point {
private:
  int width;
  int height;
  int collisionX;
  int collisionY;

public:
  Rockets(int _x, int _y);
  ~Rockets() = default;

  virtual void show();
  virtual void hide();

  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();
};

#endif
