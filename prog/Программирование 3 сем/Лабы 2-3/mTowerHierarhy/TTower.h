#ifndef TTOWER_H
#define TTOWER_H

#include "TPoint.h"

class Tower : public Point {
private:
  // Обеспечение возможности проверки коллизии
  int width;      // Ширина обьекта
  int height;     // Высота обьекта
  int collisionX; // Точка начала коллизии относительно начала обьекта
  int collisionY;

  // Специфические свойства башни
protected:
  // Необходимо для потомков
  void setWidth(int newWidth);
  void setHeight(int newHeight);
  void setCollisionX(int newX);
  void setCollisionY(int newY);

public:
  // Конструктор класса, для себя определяет его местоположение
  Tower(int _x, int _y);
  // Деструктор класса
  virtual ~Tower();

  // Показать/скрыть обьект
  virtual void show() override;
  virtual void hide() override;

  // Сеттеры

  // Геттеры
  int getWidth();
  int getHeight();
  int getCollisionX();
  int getCollisionY();
};

#endif