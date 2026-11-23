#ifndef PLANE_H
#define PLANE_H

#include "Point.h"

// Интерфейс рисунка самолёта: фюзеляж, крылья и хвост.
class IPlane {
public:
  virtual void fuselage(bool f) = 0;
  virtual void wings(bool f) = 0;
  virtual void hvost(bool f) = 0;
};

// Базовый самолёт: задаёт спрайт и хитбокс.
class Plane : public Point, public IPlane {
private:
  int width;      // ширина хитбокса
  int height;     // высота хитбокса
  int collisionX; // начало хитбокса относительно точки отрисовки
  int collisionY;

protected:
  void setWidth(int newWidth);
  void setHeight(int newHeight);
  void setCollisionX(int newX);
  void setCollisionY(int newY);

public:
  void fuselage(bool f) override;
  void wings(bool f) override;
  void hvost(bool f) override;

  Plane(int _x = 0, int _y = 0);
  virtual ~Plane();

  virtual void show() override;
  virtual void hide() override;

  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();
};

#endif
