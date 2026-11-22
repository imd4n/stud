// Фигура самолет
// Партийное задание, сделать функцию show, которая изображает нашу фигуру нарисованную
// Потом дополним point нашей фигурой и начнем философствовать
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

#include <iostream>
#include "TLocation.h"
using namespace std;           // Пространство имен std

#include "Classes.h"  //поменять в соответствии с задачей

//макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*Г Л О Б А Л Ь Н Ы Е  К О Н С Т А Н Т Ы  И  П Е Р Е М Е Н Н Ы Е*/
/****************************************************************/

HDC hdc;// Объявим контекст устройства
        // Контекст устройства это структура, содержащая
        // описание видеокарты на вашем компьютере
        // и всех необходимых графических элементов

void CollisionCheck(Square* ASquare, Square* BSquare) {
    // Проверка коллизии
    if (BSquare->getX() > ASquare->getX() && BSquare->getX() < ASquare->getX() + ASquare->getWidth()) {
        if (BSquare->getY() > ASquare->getY() && BSquare->getY() < ASquare->getY() + ASquare->getHeight()) {
            cout << "collision" << endl;
        }
        else {
                cout << "no collision" << endl;
        }
    }
    else {
        cout << "no collision" << endl;
    }
}

/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/
int main()
{
    //объявление и инициализация переменных

    //для классов LOCATION и POINT
    int x0 = 300;	//стартовые координаты ТОЧКИ
    int y0 = 300;

    //для класса CIRCLE
    int Radius0=50;		//начальное значение радиуса КРУГА
    int DeltaRad=30;		//шаг изменения радиуса КРУГА

    //для класса RING
    int RingW0 = 20;		//начальное значение толщины КОЛЬЦА
    int DeltaWide0 = 10;	//шаг изменения толщины КОЛЬЦА


    //получим дескриптор консольного окна
    HWND hwnd = GetConsoleWindow();

    //если дескриптор НЕ существует
    if (hwnd == NULL)
    {
        cout << "Console Window Descriptor NOT FOUND !\n";
        return 1; //ERROR
    }//if

    //дескриптор существует

    //получим контекст устройства для консольного окна
    hdc = GetWindowDC(hwnd);

    //если контекст НЕ существует
    if (hdc == 0)
    {
        cout << "Handle Device Context NOT FOUND !\n";
        return 2; //ERROR
    }//if

   //контекст существует - можем работать
//===============================================================
//	М Е С Т О    В С Т А В К И
    // Создаем и показываем самолет
    //Plane APlane(x0, y0);
    //APlane.Show();

    //cout << "Press Any key ";
    //getchar();

    // Двигаем самолетик
    //APlane.moveTo(x0+100, y0+100);

    // Проверка колизий
    // Для того, чтобы проверить колизию, нужно написать функцию, которая обрабатывает пересечение с каждым обьектом, для этого, надо вызвать у каждого существующего обьекта его линии
    // Чтобы это сделать, надо

    // Создали квадрат
    /*Plane APlane(x0, y0);
    Plane BPlane(x0+200, y0+200);
    APlane.Show();
    BPlane.Show();

    CollisionCheck(APlane.getCollisionObj(), BPlane.getCollisionObj());
    getchar();

    BPlane.moveTo(x0+40, y0+40);
    CollisionCheck(APlane.getCollisionObj(), BPlane.getCollisionObj());
*/
    Circle ACircle(x0, y0, 30);
    ACircle.Show();

    getchar();

    ACircle.moveTo(x0+100, y0+100);

while(1) {}
     cout << "\nExample ExOOP_0x DONE   \7\n";
//	К О Н Е Ц     В С Т А В К И
//===============================================================
return 0;

}//end main()

/****************  End Of ExOOP_ШАБЛОН.СPP File ****************/
