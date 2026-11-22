#ifndef TPLANE_H
#define TPLANE_H

#include "TPoint.h"

// Интерфейс самолета
class IPlane {
public:
  virtual void fuselage(bool f) = 0;
  virtual void wings(bool f) = 0;
  virtual void hvost(bool f) = 0;
  virtual void rockets(bool f) = 0;
};

// Класс самолета
class Plane : public Point, public IPlane {
private:
  // Обеспечение возможности проверки коллизии
  int width;      // Ширина обьекта
  int height;     // Высота обьекта
  int collisionX; // Точка начала коллизии относительно начальной точки
                  // отрисовки
  int collisionY;

protected:
  // Необходимо для потомков
  void setWidth(int newWidth);
  void setHeight(int newHeight);
  void setCollisionX(int newX);
  void setCollisionY(int newY);

  int ID = 0; // Идентификатор обьекта

public:
  void fuselage(bool f) override;
  void wings(bool f) override;
  void hvost(bool f) override;
  void rockets(bool f) override;

  // Конструктор класса, для себя определяет его местоположение
  Plane(int _x, int _y);
  // Деструктор класса
  virtual ~Plane() = default;

  // Показать/скрыть обьект
  virtual void show() override;
  virtual void hide() override;

  // Геттеры
  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();
  int getID() const { return ID; }
};

class AttackPlane : public Plane {
public:
  AttackPlane(int _x, int _y);
  virtual ~AttackPlane() = default;

  void rockets(bool f) override;
};

#endif