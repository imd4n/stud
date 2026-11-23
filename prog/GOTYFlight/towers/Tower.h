#ifndef TOWER_H
#define TOWER_H

#include "Point.h"

// Дом-препятствие: рисуется от основания (x, y) вверх.
class Tower : public Point {
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

  int ID = 0; // тип спрайта: 0 — обычный, 1 — магический, 2 — маленький

public:
  Tower(int _x = 0, int _y = 0);
  virtual ~Tower();

  virtual void show() override;
  virtual void hide() override;

  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();
  virtual int getID() { return ID; }
};

// Упрощённый дом: только контур, без этажей и окон.
class SmallTower : public Tower {
public:
  SmallTower(int _x = 0, int _y = 0);
  virtual ~SmallTower() = default;

  void show() override;
};

#endif
