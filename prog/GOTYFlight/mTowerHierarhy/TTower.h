#ifndef TTOWER_H
#define TTOWER_H

#include "TPoint.h"
#include <cmath>

class Tower : public Point {
private:
  // Обеспечение возможности проверки коллизии
  int width;      // Ширина обьекта
  int height;     // Высота обьекта
  int collisionX; // Точка начала коллизии относительно начала обьекта
  int collisionY;

protected:
  // Необходимо для потомков
  void setWidth(int newWidth);
  void setHeight(int newHeight);
  void setCollisionX(int newX);
  void setCollisionY(int newY);

  int ID = 0;
  bool visible = 0;
  int damage = 10;

  int towerSpeed = 7;

public:
  // Конструктор класса, для себя определяет его местоположение
  Tower(int _x = 0, int _y = 0);
  // Деструктор класса
  virtual ~Tower();

  // Показать/скрыть обьект
  virtual void show() override;
  virtual void hide() override;

  // Сеттеры
  void setVisible(bool v) { visible = v; }

  // Геттеры
  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();

  virtual void moveTo(int _x, int _y) {
    hide();
    x = _x;
    y = _y;
    show();
  }

  virtual void move() { moveTo(x, y + towerSpeed); }

  virtual int getID() { return ID; }
  virtual bool getVisible() { return visible; }
  int getDamage() { return damage; }
};

class SmallTower : public Tower {
private:
  double r = 100; // радиус окружности
  double phi = 0; // текущий угол
  double centerX; // центр вращения по X
  double centerY; // центр вращения по Y

public:
  SmallTower(int _x = 0, int _y = 0);
  virtual ~SmallTower() = default;

  void show() override;

  virtual void moveTo(int _x, int _y) override {
    hide();
    x = _x;
    y = _y;
    // Устанавливаем новый центр вращения и обнуляем угол
    centerX = _x;
    centerY = _y;
    phi += 0.1;
    show();
  }

  void move() override {
    hide();
    centerY += towerSpeed;

    x = centerX + r * cos(phi);
    y = centerY + r * sin(phi);

    phi += 0.1;

    show();
  }
};

#endif