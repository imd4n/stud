#ifndef TPLANE_H
#define TPLANE_H

#include "TPoint.h"

class IPlane {
public:
  virtual void fuselage(bool f) = 0;
  virtual void wings(bool f) = 0;
  virtual void hvost(bool f) = 0;
  virtual void rocket1(bool f) = 0;
  virtual void rocket2(bool f) = 0;
};

#include "../mObjects/TRockets.h"
// Класс самолета, явялется интерфейсом, не подразумевает реализации
// Не определяет обьект, а лишь его базовые возможности для создания
// конкретных реализаций в других классах
class Plane : public Point, public IPlane {
private:
  // Обеспечение возможности проверки коллизии
  int width;      // Ширина обьекта
  int height;     // Высота обьекта
  int collisionX; // Точка начала коллизии относительно начальной точки
                  // отрисовки
  int collisionY;

  // Специфические свойства самолета
protected:
  // Необходимо для потомков
  void setWidth(int newWidth);
  void setHeight(int newHeight);
  void setCollisionX(int newX);
  void setCollisionY(int newY);

  int ID = 0;
  bool Visible;

public:
  void fuselage(bool f) override;
  void wings(bool f) override;
  void hvost(bool f) override;
  void rocket1(bool f) override {};
  void rocket2(bool f) override {};

  // Конструктор класса, для себя определяет его местоположение
  Plane(int _x = 0, int _y = 0);
  // Деструктор класса
  virtual ~Plane();

  // Показать/скрыть обьект
  virtual void show() override;
  virtual void hide() override;

  // Сеттеры

  // Геттеры
  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();
  int getID() const { return ID; }

  virtual void shoot() {}
  virtual Rockets *getr(int num) { return nullptr; }
  virtual void setRockets(int inc) {}
};

class AttackPlane : public Plane {
private:
  // Обьекты типа ракета, обрабатываются и делаются сами
  Rockets *r[2];
  int rocketnum = 2;

public:
  AttackPlane(int _x = 0, int _y = 0);
  virtual ~AttackPlane();

  // Для него мы хотим переопределить show, чтобы показывать ракеты
  void show() override;

  void rocket1(bool f) override;
  void rocket2(bool f) override;

  Rockets *getr(int num) override {
    if (num >= 0 && num < 2)
      return r[num];
    return nullptr;
  }

  void shoot() override {
    for (int i = 0; i < 2; i++) {
      if (rocketnum > 0 && !r[i]->getVisible()) {
        rocketnum--;
        r[i]->moveTo(x, y + 40);
        r[i]->setVisible(true);
        break;
      }
    }
  }

  void setRockets(int inc) override;
};

#endif