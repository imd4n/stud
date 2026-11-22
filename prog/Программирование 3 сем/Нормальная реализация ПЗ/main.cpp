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

#include "./mPlaneHierarhy/TPlane.h"

#include <iostream>
#include <winscard.h>
using namespace std;           // Пространство имен std

//макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*Г Л О Б А Л Ь Н Ы Е  К О Н С Т А Н Т Ы  И  П Е Р Е М Е Н Н Ы Е*/
/****************************************************************/

HDC hdc;// Объявим контекст устройства
        // Контекст устройства это структура, содержащая
        // описание видеокарты на вашем компьютере
        // и всех необходимых графических элементов

/*void CollisionCheck(Square* ASquare, Square* BSquare) {
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
}*/

template<typename T1, typename T2>
bool isCollision(T1& obj1, T2& obj2) {
    // Получаем координаты левого нижнего угла и размеры первого объекта
    double x1 = obj1.getCollisionX();
    double y1 = obj1.getCollisionY();
    double w1 = obj1.getWidth();
    double h1 = obj1.getHeight();
    
    // Получаем координаты левого нижнего угла и размеры второго объекта
    double x2 = obj2.getCollisionX();
    double y2 = obj2.getCollisionY();
    double w2 = obj2.getWidth();
    double h2 = obj2.getHeight();
    
    // Проверяем пересечение по осям X и Y
    bool collisionX = (x1 + w1 >= x2) && (x2 + w2 >= x1);
    bool collisionY = (y1 + h1 >= y2) && (y2 + h2 >= y1);
    
    // Столкновение есть если есть пересечение по обеим осям
    return collisionX && collisionY;
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
    //Tower ATower(x0+600, y0+300);
    //ATower.show();
    
    Plane APlane(x0, y0);
    APlane.show();
    
    getchar();


    APlane.moveTo(x0+100, y0+100);
    //int FigX = x0+600;
    //int FigY = y0+400;
    //int Step = 5;
    while(1) {
    /*    if (KEY_DOWN(38)) // arrow up
        {
	        FigY-=Step;
            APlane.moveTo(FigX, FigY);
	        Sleep(50);
        }
        if (KEY_DOWN(40)) // arrow down 
        {
	        FigY+=Step;
            APlane.moveTo(FigX, FigY);
	        Sleep(50);
        }
        if (KEY_DOWN(37)) // arrow left 
        {
	        FigX-=Step;
            APlane.moveTo(FigX, FigY);
	        Sleep(50);
        }
        if (KEY_DOWN(39)) // arrow right
        {
	        FigX+=Step;
            APlane.moveTo(FigX, FigY);
	        Sleep(50);
        }
        
        // Проверка коллизии
        if (isCollision(APlane, ATower)) {
            cout << "collision" << endl;
            break;
        }*/
    }
    getchar();
    cout << "\nExample ExOOP_0x DONE   \7\n";
//	К О Н Е Ц     В С Т А В К И
//===============================================================
return 0;

}//end main()

/****************  End Of ExOOP_ШАБЛОН.СPP File ****************/
