#pragma once

/********************************************************************
 *          ЛАБОРАТОРНАЯ РАБОТА ПО ООП 4 СЕМЕСТР                    *
 *------------------------------------------------------------------*
 * Project Type  : Win64 Console Application                        *
 * Project Name  : Bus	                                            *
 * File Name     : Bus.h	                                        *
 * Programmer(s) : Балакшин Артём М3О-207Б-23                       *
 * Modifyed By   :                                                  *
 * Created       : 08/04/2025                                       *
 * Last Revision : 27/04/2025                                       *
 * Comment(s)    : ПРОСТЕЙШАЯ ИЕРАРХИЯ КЛАССОВ						*
 *******************************************************************/

/*-----------------------------------------------------------------------*/
/*                           ОБЪЯВЛЕНИЕ КЛАССОВ                          */
/*-----------------------------------------------------------------------*/

/*-----------------------  Класс Location -------------------------*/
class Location {
protected:
	int X; //координата х
	int Y; //координата y
public:
	Location(int InitX, int InitY) { X = InitX; Y = InitY; } //конструктор
	~Location() = default;                                   //деструктор
	int GetX() { return X; }								 //геттер х
	int GetY() { return Y; }								 //геттер y
	void SetX(int NewX) { X = NewX; };						 //сеттер x
	void SetY(int NewY) { Y = NewY; };						 //сеттер y

};

/*-----------------------  Класс Point ----------------------------------*/
class Point : public Location {
protected:
	bool Visible; //видимость точки

public:
	Point(int InitX, int InitY) : Location(InitX, InitY) { Visible = false; } //конструктор
	~Point() = default;														  //деструктор

	bool IsVisible() { return Visible; }									  //видима ли точка

	void Drag(int Step);													  //буксировка

	virtual void MoveTo(int NewX, int NewY);								  //переместить
	virtual void Show();													  //показать точку
	virtual void Hide();													  //спрятать точку
};

/*-----------------------  Абстрактный базовый класс ABC_Figure ---------------------------*/
class ABC_Figure : public Point {
protected:
	int rightBorder;  //правая граница фигуры
	int leftBorder;	  //левая граница фигуры
	int topBorder;	  //верхняя граница фигуры
	int bottomBorder; //нижняя граница фигуры
	int index;        //индекс фигуры

public:
	ABC_Figure(int InitX, int InitY) : Point(InitX, InitY) {}; //конструктор с параметром
	ABC_Figure() : Point(0, 0) {};							   //конструктор без параметра
	~ABC_Figure() = default;								   //декструктор
	int getRightBorder() { return rightBorder; }		   //геттер правой границы
	int getLeftBorder() { return leftBorder; }			   //геттер левой границы
	int getTopBorder() { return topBorder; }			   //геттер верхней границы
	int getBottomBorder() { return bottomBorder; }		   //геттер нижней границы
	int getIndex() { return index; }					   //геттер индекса

	void Show() override;								   //переопределение показать
	void Hide() override;								   //переопределение скрыть
	virtual void Draw(COLORREF color) = 0;				   //абстрактное рисовать фигуру
	void MoveTo(int newX, int newY) override;			   //переопределение переместить
	virtual void UpdateHitbox() = 0;					   //абстрактное обновить границы фигуры
	void DrawHitbox(COLORREF color);					   //нарисовать границы фигуры
};

/*-----------------------  Интерфейс IBus ----------------------------------*/
class IBus {
public:
	virtual void DrawCab(COLORREF color) = 0;			  //абстрактный нарисовать кабину
	virtual void DrawBody(COLORREF color) = 0;			  //абстрактный нарисовать корпус
	virtual void DrawWheels(COLORREF color) = 0;		  //абстрактный нарисовать колёса
	virtual void DrawWheel(int centerX, int centerY) = 0; //абстрактный нарисовать одно колесо
};

/*-----------------------  Класс Bus ----------------------------------*/
class Bus : public ABC_Figure, public IBus {
protected:
	int BodyWidth;		 //ширина корпуса автобуса
	int BodyHeight;		 //высота корпуса автобуса
	int WheelRadius;	 //радиус колес
	int CabWidth;		 //ширина кабины
	int CabHeight;		 //высота кабины
public:
	Bus(int InitX = 200, int InitY = 200); //конструктор с параметрами
	~Bus() = default;																		//деструктор

	void Draw(COLORREF color) override;												//принудительная реализация рисовать автобус
	void DrawWheels(COLORREF color) override;										//принудительная реализация рисовать колёса
	void DrawWheel(int centerX, int centerY) override;								//принудительная реализация рисовать колесо
	void DrawBody(COLORREF color) override;											//принудительная реализация рисовать корпус 
	void DrawCab(COLORREF color) override;											//принудительная реализация рисовать кабину
	void UpdateHitbox() override;													//принудительная реализация обновить границы автобуса
};

/*-----------------------  Класс MiniBus ----------------------------------*/
class MiniBus : public Bus {
public:
	MiniBus(int InitX = 200, int InitY = 200); //конструктор с параметрами
	~MiniBus() = default;																//деструктор
};

/*-----------------------  Класс ThreeWheelBus ----------------------------------*/
class ThreeWheelBus : public Bus {
public:
	ThreeWheelBus(int InitX = 200, int InitY = 200); //конструктор с параметрами
	~ThreeWheelBus() = default;					     //деструктор

	void DrawWheels(COLORREF color) override;												  //переопределённое рисовать колёса
	void DrawBody(COLORREF color) override;													  //переопределённое рисовать корпус
};


/*-----------------------  Класс ThreeWheelBus ----------------------------------*/
class ThreeWheelBusWithTower : public ThreeWheelBus {
public:
	ThreeWheelBusWithTower(int InitX = 200, int InitY = 200);//конструктор с параметрами
	~ThreeWheelBusWithTower() = default;					     //деструктор

	void DrawTower(COLORREF color);
	void DrawTube(COLORREF color);
	void UpdateHitbox();
	void Draw(COLORREF color) override;
};

/*-----------------------  Класс SquareBus ----------------------------------*/
class SquareBus : public Bus {
public:
	SquareBus(int InitX = 200, int InitY = 200); //конструктор с параметрами
	~SquareBus() = default;																  //деструктор

	void DrawWheel(int centerX, int centerY) override;									  //переопределённое рисовать колесо
	void DrawBody(COLORREF color) override;												  //переопределённое рисовать корпус
	void UpdateHitbox() override;														  //переопределённое изменить границы SquareBus
};

/*-----------------------  Класс Stone ----------------------------------*/
class Stone : public ABC_Figure {
protected:
	int BodyHeight; //Высота корпуса
	int BodyWidth;  //Ширина корпуса
public:
	Stone(int InitX, int InitY, int InitHeight, int InitWidth); //конструктор с парметрами
	~Stone() = default;											//деструктор
	void Draw(COLORREF color) override;							//переопределённое рисовать камень
	void UpdateHitbox() override;								//переопределённое изменить границы камня
};

/*-----------------------  Класс RepairKit ----------------------------------*/
class RepairKit : public ABC_Figure {
protected:
	int BodyHeight; //Высота корпуса
	int BodyWidth;  //Ширина корпуса
public:
	RepairKit(int InitX, int InitY, int InitHeight, int InitWidth); //конструктор с парметрами
	~RepairKit() = default;											//деструктор
	void Draw(COLORREF color) override;								//переопределённое рисовать ремонтный набор
	void UpdateHitbox() override;									//переопределённое изменить границы
};

/*-----------------------  Класс TrinityKit ----------------------------------*/
class TrinityKit : public ABC_Figure {
protected:
	int BodyHeight; //Высота корпуса
	int BodyWidth;  //Ширина корпуса
public:
	TrinityKit(int InitX, int InitY, int InitHeight, int InitWidth); //конструктор с параметрами
	~TrinityKit() = default;										 //деструктор
	void Draw(COLORREF color) override;							     //переопределённое рисовать утроенный набор
	void UpdateHitbox() override;									 //переопределённое изменить границы
};


/*-----------------------------------------------------------------------*/
/*                           ОБЪЯВЛЕНИЕ ФУНКЦИЙ                          */
/*-----------------------------------------------------------------------*/
//Вывод всей иерархии объектов 
void PrintAllObjects();

//симуляция столкновений
void CollisionSimulation(Bus* currBus, ABC_Figure* conflictObjects[], Bus** buses,
	int conflictObjectCount, int planeCount, int** conflictResults);

//Проверка столкновения 
bool HadCollided(Bus* currBus, ABC_Figure* conflictObject);


