// Фигура самолет
// Партийное задание, сделать функцию show, которая изображает нашу фигуру
// нарисованную Потом дополним point нашей фигурой и начнем философствовать
/******************************************************************
 * О Б Ъ Е К Т Н О - О Р И Е Н Т И Р О В А Н Н О Е ПРОГРАММИРОВАНИЕ*
 *-----------------------------------------------------------------*
 * Project Type  : QT Console Application                          *
 * Project Name  : PRINCIPES_ООП\                                  *
 * File Name     : ExOOP_ШАБЛОН.СPP                                *
 * Language      : C++ QT ver. 5.12.2 Qt Creator 4.8.2.            *
 * Programmer(s) : Чечиков Ю.Б.  &  Секретарев В.Е.                *
 * Modified By   : Чечиков Ю.Б.                                    *
 * Created       : 26/04/04                                        *
 * Last Revision : 25/01/25                                        *
 * Comment(s)    : ЗАГОТОВКА ДЛЯ РАБОТЫ В КОНСОЛЬНОМ ОКНЕ          *
 *                                                                 *
 ******************************************************************/

#include <windows.h>

#include "./mPlaneHierarhy/TBrokenPlane.h"
#include "./mPlaneHierarhy/TMagicPlane.h"
#include "./mPlaneHierarhy/TPlane.h"
#include "./mTowerHierarhy/TTower.h"
#include "./mTowerHierarhy/TMagicTower.h"
#include "mPlaneHierarhy/TRepairedPlane.h"

#include <iostream>
#include <winscard.h>
using namespace std; // Пространство имен std

// макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*Г Л О Б А Л Ь Н Ы Е  К О Н С Т А Н Т Ы  И  П Е Р Е М Е Н Н Ы Е*/
/****************************************************************/

HDC hdc; // Объявим контекст устройства
         // Контекст устройства это структура, содержащая
         // описание видеокарты на вашем компьютере
         // и всех необходимых графических элементов


bool isCollision(Plane *obj1, Tower *obj2) {
  // Получаем координаты левого верхнего угла и размеры первого объекта
  double x1 = obj1->getCollisionX();
  double y1 = obj1->getCollisionY();
  double w1 = obj1->getWidth();
  double h1 = obj1->getHeight();

  // Получаем координаты левого верхнего угла и размеры второго объекта
  double x2 = obj2->getCollisionX();
  double y2 = obj2->getCollisionY();
  double w2 = obj2->getWidth();
  double h2 = obj2->getHeight();

  // Проверяем пересечение по осям X и Y
  bool collisionX = (x1 + w1 >= x2) && (x2 + w2 >= x1);
  bool collisionY = (y1 + h1 >= y2) && (y2 + h2 >= y1);

  // Столкновение есть если есть пересечение по обеим осям
  return collisionX && collisionY;
}
/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/
int main() {
  // объявление и инициализация переменных

  // для классов LOCATION и POINT
  int x0 = 300; // стартовые координаты ТОЧКИ
  int y0 = 300;

  // для класса CIRCLE
  int Radius0 = 50;  // начальное значение радиуса КРУГА
  int DeltaRad = 30; // шаг изменения радиуса КРУГА

  // для класса RING
  int RingW0 = 20;     // начальное значение толщины КОЛЬЦА
  int DeltaWide0 = 10; // шаг изменения толщины КОЛЬЦА

  // получим дескриптор консольного окна
  HWND hwnd = GetConsoleWindow();

  // если дескриптор НЕ существует
  if (hwnd == NULL) {
    cout << "Console Window Descriptor NOT FOUND !\n";
    return 1; // ERROR
  } // if

  // дескриптор существует

  // получим контекст устройства для консольного окна
  hdc = GetWindowDC(hwnd);

  // если контекст НЕ существует
  if (hdc == 0) {
    cout << "Handle Device Context NOT FOUND !\n";
    return 2; // ERROR
  } // if

  // контекст существует - можем работать
  //===============================================================
  //	М Е С Т О    В С Т А В К И
  // Обьект башни
  Tower ATower(x0 + 600, y0 + 300);
  MagicTower BTower(x0, y0 + 300);
  ATower.show();
  BTower.show();

  // Обьект самолет
  // Plane *APlane = new Plane(x0 + 600, y0 + 500);
  // MagicPlane *BPlane = new MagicPlane(x0 + 600, y0 + 500);
  // BrokenPlane *CPlane = new BrokenPlane(x0 + 600, y0 + 500);
  // RepairedPlane *DPlane = new RepairedPlane(x0 + 600, y0 + 500);

  // APlane->show();

  // Обеспечение перемещения обьекта
  int FigX = x0 + 600;
  int FigY = y0 + 500;
  int Step = 5;

  // Указатели на текущие используемые обьекты
  // Plane *plane = APlane;
  Tower *tower = &ATower;
  Tower *tower2 =&BTower;

  // Переменная, помнящая последнюю нажатую кнопку, чтобы знать куда откинуть
  // самолет
  char lastButton = 0;

  // Обеспечение изменения обьекта через матрицу
  // Матрица указателей на обьекты
  /*Plane *objSwap[4][2] = {{&APlane, &BPlane}, 
                          {&BPlane, &CPlane}, 
                          {&CPlane, &DPlane}, 
                          {&DPlane, &APlane}};
  */
  // Plane *objSwap[4] = {APlane, BPlane, CPlane, DPlane};
  Plane* objSwap[] =  {
                        new MagicPlane(x0 + 600, y0 + 500),
                        new BrokenPlane(x0 + 600, y0 + 500),
                        new RepairedPlane(x0 + 600, y0 + 500),
                        new Plane(x0 + 600, y0 + 500)
  };
  Plane* plane = new Plane(x0 + 600, y0 + 500);
  plane->show();
  int pickedPlane = 1;

  while (1) {
    /* Движение обьекта */
    if (KEY_DOWN(38)) // arrow up
    {
      // Запомним кнопку
      lastButton = 38;

      FigY -= Step;
      plane->moveTo(FigX, FigY);
      Sleep(50);
    }
    if (KEY_DOWN(40)) // arrow down
    {
      // Запомним кнопку
      lastButton = 40;

      FigY += Step;
      plane->moveTo(FigX, FigY);
      Sleep(50);
    }
    if (KEY_DOWN(37)) // arrow left
    {
      // Запомним кнопку
      lastButton = 37;

      FigX -= Step;
      plane->moveTo(FigX, FigY);
      Sleep(50);
    }
    if (KEY_DOWN(39)) // arrow right
    {
      // Запомним кнопку
      lastButton = 39;

      FigX += Step;
      plane->moveTo(FigX, FigY);
      Sleep(50);
    }

    // Проверка коллизии c башней
    if (isCollision(plane, tower) ){
      // Скрыли старый самолет
      plane->hide();

      // Установивили самолет, который появляется после столкновения с башней
      /*if (pickedPlane == 4) {
        plane = objSwap[0][0];
        pickedPlane = 1;
      } else if (pickedPlane == 1) {
        plane = objSwap[1][0];
        pickedPlane = 2;
      } else if (pickedPlane == 2) {
        plane = objSwap[2][0];
        pickedPlane = 3;
      }*/
      plane = objSwap[plane->getID()];
      
      // Переместили обьект в зависимости от стороны удара (соответственно и
      // показали)
      if (lastButton == 38) // удар снизу
        FigY = FigY + 50;
      else if (lastButton == 40) // удар сверху
        FigY = FigY - 50;
      else if (lastButton == 39) // удар справа
        FigX = FigX - 50;
      else if (lastButton == 37) // удар слева
        FigX = FigX + 50;
      plane->moveTo(FigX, FigY);
    }

    if (isCollision(plane, tower2)) {

      // Переместили обьект в зависимости от стороны удара (соответственно и
      // показали)
      if (lastButton == 38) // удар снизу
        FigY = FigY + 50;
      else if (lastButton == 40) // удар сверху
        FigY = FigY - 50;
      else if (lastButton == 39) // удар справа
        FigX = FigX - 50;
      else if (lastButton == 37) // удар слева
        FigX = FigX + 50;
      plane->moveTo(FigX, FigY);

    }
  }
  cout << "\nExample ExOOP_0x DONE   \7\n";
  //	К О Н Е Ц     В С Т А В К И
  //===============================================================
  return 0;

} // end main()

/****************  End Of ExOOP_ШАБЛОН.СPP File ****************/
