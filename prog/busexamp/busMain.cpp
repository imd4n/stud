/********************************************************************
 *          ЛАБОРАТОРНАЯ РАБОТА ПО ООП 4 СЕМЕСТР                    *
 *------------------------------------------------------------------*
 * Project Type  : Win64 Console Application                        *
 * Project Name  : Bus	                                            *
 * File Name     : busMain.cpp                                      *
 * Programmer(s) : Балакшин Артём М3О-207Б-23                       *
 * Modifyed By   :                                                  *
 * Created       : 08/04/2025                                       *
 * Last Revision : 27/04/2025                                       *
 * Comment(s)    : ПРОСТЕЙШАЯ ИЕРАРХИЯ КЛАССОВ						*
 *******************************************************************/

#include <windows.h> //WinApi (для работы с консолью и графикой)
#include <iostream>
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
using namespace std;           //  Стандартное пространство имен 
#include "bus.h"

HDC hdc;// Объявим контекст устройства
        // Контекст устройства это структура, содержащая
        // описание видеокарты на вашем компьютере
        // и всех необходимых графических элементов

int main()
{
    //получим дескриптор консольного окна
    HWND hwnd = GetConsoleWindow();

    //если дескриптор НЕ существует
    if (hwnd == NULL)
    {
        cout << "Console Window Descriptor NOT FOUND !\n";
        return 1; //ERROR
    }

    //дескриптор существует

    //получим контекст устройства для консольного окна
    hdc = GetWindowDC(hwnd);

    //если контекст НЕ существует
    if (hdc == 0)
    {
        cout << "Handle Device Context NOT FOUND !\n";
        return 2;
    }

    //Вывести всю иерархию объектов 
//    PrintAllObjects();

    
    //Количество автобусов и конфликтующих объектов 
    int const BUS_QUANTITY = 5;  
    int const CONFLICT_OBJECT_QUANTITY = 3;

    //Создание объектов автобусов (разные типы, но с одинаковыми начальными параметрами)
    Bus bus(100, 250);
    MiniBus miniBus(100, 250);
    ThreeWheelBus threeWhellBus(100, 250);
    SquareBus squareBus(100, 250);
    ThreeWheelBusWithTower threeWheelBusWithTower(100, 250);

    //Массив указателей на автобусы (для полиморфного вызова методов)
    Bus* busArr[BUS_QUANTITY] = { &bus, &miniBus, &threeWhellBus, &squareBus, &threeWheelBusWithTower};

    //Текущий выбранный самолёт (по умолчанию - первый)
    Bus* currBus;
    currBus = &bus;

    //Создание объектов, с которыми может столкнуться автобус
    Stone stone(700, 90, 100, 100);
    RepairKit repairKit(700, 360, 100, 100);
    TrinityKit trinityKit(700, 630, 100, 100);

    //Массив указателей на конфликтные объекты (полиморфизм через базовый класс Figure)
    ABC_Figure* conflictObjectArr[CONFLICT_OBJECT_QUANTITY] = { &stone, &repairKit, &trinityKit };

    //Статический массив с результатами столкновений (заранее заданные значения)
    int conflict_result_stat[BUS_QUANTITY][CONFLICT_OBJECT_QUANTITY] = {
        {3, 1, 2},  //Результаты для bus
        {3, 0, 0},  //Результаты для miniBus
        {4, 3, 2},  //Результаты для threeWheelBus
        {3, 0, 4},  //Результаты для squareBus
        {3, 1, 2},  //Результаты для threeWheelBusWithTube
    };

    //Динамический массив (указатели на строки conflict_result_stat)
    int** conflict_result = new int* [BUS_QUANTITY];
    for (int i = 0; i < BUS_QUANTITY; i++) {
        conflict_result[i] = conflict_result_stat[i];
    }

    //Запуск симуляции столкновений
    CollisionSimulation(currBus,                //Текущий автобус 
                        conflictObjectArr,        //Массив препятствий
                        busArr,                  //Массив всех автобусов
                        CONFLICT_OBJECT_QUANTITY,  //Количесвто конфликтующих объектов
                        BUS_QUANTITY,             //Количество автобусов
                        conflict_result             //Матрица результатов столкновений
                        );
    //Освобождение динамической памяти
    delete[] conflict_result;  
    conflict_result = nullptr; 
    return 0;
}

