#pragma once

/********************************************************************
 *          ЛАБОРАТОРНАЯ РАБОТА ПО ООП 4 СЕМЕСТР                    *
 *------------------------------------------------------------------*
 * Project Type  : Win64 Console Application                        *
 * Project Name  : Bus	                                            *
 * File Name     : Bus.cpp	                                        *
 * Programmer(s) : Балакшин Артём М3О-207Б-23                       *
 * Modifyed By   :                                                  *
 * Created       : 08/04/2025                                       *
 * Last Revision : 27/04/2025                                       *
 * Comment(s)    : ПРОСТЕЙШАЯ ИЕРАРХИЯ КЛАССОВ						*
 *******************************************************************/

#include <windows.h>
#include <iostream>
#include "bus.h" //объявление классов

using namespace std; //пространство имён std

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) //Распознование нажатых клавиш 

extern HDC hdc; //контекст устройства

/*----------------------------------------------------------------*/
/*             Р Е А Л И З А Ц И Я    М Е Т О Д О В               */
/*----------------------------------------------------------------*/

/*-----------------------  Класс Point ----------------------------------*/

//Показать Point
void Point::Show()
{
	Visible = true; //точку видно
	SetPixel(hdc, X, Y, RGB(255, 0, 0)); //рисуем КРАСНЫМ цветом

	//показываем 4 точки, чтобы было лучше видно на экране
	SetPixel(hdc, X + 1, Y, RGB(255, 0, 0)); //рисуем установленным цветом
	SetPixel(hdc, X, Y + 1, RGB(255, 0, 0)); //рисуем установленным цветом
	SetPixel(hdc, X + 1, Y + 1, RGB(255, 0, 0)); //рисуем установленным цветом
}

//Скрыть Point
void Point::Hide()
{
	Visible = false; //точку НЕ видно

	//чтобы было видно работу функции, погасшую точку будем рисовать
	//СИНИМ цветом. В общем случае, конечно, надо рисовать цветом фона

	//гасим 4 точки
	SetPixel(hdc, X, Y, RGB(0, 0, 255)); //рисуем цветом фона
	SetPixel(hdc, X + 1, Y, RGB(0, 0, 255)); //рисуем цветом фона
	SetPixel(hdc, X, Y + 1, RGB(0, 0, 255)); //рисуем цветом фона
	SetPixel(hdc, X + 1, Y + 1, RGB(0, 0, 255)); //рисуем цветом фона

}//Point::Hide()

//переместить точку по новым координатам
void Point::MoveTo(int NewX, int NewY)
{
	Hide();    //скрыть точку по текущим координатам
	X = NewX;  //поменять координаты
	Y = NewY;
	Show();    //показать точку по новым координатам
}

//Перетаскивать Point
void Point::Drag(int Step) {
	int NewX = GetX(); //получаем начальное положение
	int NewY = GetY();

	while (true) { //бесконечный цикл буксировки
		//esc - выход
		if (KEY_DOWN(VK_ESCAPE)) {
			break;
		}

		//перемещение фигуры в зависимости от нажатой стрелки
		if (KEY_DOWN(VK_LEFT)) { //стрелка влево 37
			NewX -= Step;       //вычисление новой координаты
			MoveTo(NewX, NewY); //переместить точку
		}

		if (KEY_DOWN(VK_RIGHT)) //стрелка вправо  39
		{
			NewX += Step;
			MoveTo(NewX, NewY);
		}

		if (KEY_DOWN(VK_UP)) //стрелка вверх  38
		{
			NewY -= Step;
			MoveTo(NewX, NewY);
		}

		if (KEY_DOWN(VK_DOWN)) //стрелка вниз  40
		{
			NewY += Step;
			MoveTo(NewX, NewY);
		}
		//Sleep(10); //задержка
	}
}

/*---------------------- Абстрактный Класс ABC_Figure ---------------------*/
//Показать Figure
void ABC_Figure::Show() {
	Visible = true;
	Draw(RGB(255, 255, 255));
}

//Скрыть Figure
void ABC_Figure::Hide() {
	Visible = false;
	DrawHitbox(RGB(12, 12, 12));
}

//Нарисовать границы объекта
void ABC_Figure::DrawHitbox(COLORREF color) {
	HPEN blackPen = CreatePen(PS_SOLID, 1, color);
	HBRUSH blackBrush = CreateSolidBrush(color);
	SelectObject(hdc, blackPen);
	SelectObject(hdc, blackBrush);
	Rectangle(hdc, leftBorder, topBorder, rightBorder, bottomBorder);  //рисуем по крайним координатам хитбокса
	DeleteObject(blackPen);
	DeleteObject(blackBrush);
}

//Переместить с изменением границ объекта
void ABC_Figure::MoveTo(int newX, int newY) {
	Hide();    //скрыть точку по текущим координатам
	X = newX;  //поменять координаты
	Y = newY;
	UpdateHitbox();
	Show();    //показать точку по новым координатам
}

/*----------------------------- Класс Bus ------------------------*/
//конструктор
Bus::Bus(int InitX, int InitY) : ABC_Figure(InitX, InitY)
{
	//инициализация параметров автобуса
	BodyWidth = 180;
	BodyHeight = 60;
	WheelRadius = 18;
	CabWidth = 60;
	CabHeight = 30;

	//установка границ объекта
	UpdateHitbox();
	index = 0;		//присовение индекса соответсвующего объекту
}

//Нарисовать весь автобус
void Bus::Draw(COLORREF color) {
	DrawBody(RGB(255, 255, 0));     //корпус
	DrawCab(RGB(200, 200, 0));		//кабина
	DrawWheels(RGB(150, 150, 150)); //колёса
}

//Нарисовать корпус автобуса
void Bus::DrawBody(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	//корпус автобуса
	Rectangle(hdc, X - BodyWidth / 2, Y - BodyHeight / 2, X + BodyWidth / 2, Y + BodyHeight / 2);
	DeleteObject(pen);
	DeleteObject(brush);
}

//Рисовать кабину
void Bus::DrawCab(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Rectangle(hdc, X + BodyWidth / 2, Y, X + BodyWidth / 1.5, Y + BodyHeight / 2);
	DeleteObject(pen);
	DeleteObject(brush);
}

//Рисовать колёса
void Bus::DrawWheels(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	int leftWheelCenterX = X - BodyWidth / 4;
	int wheelsCenterY = Y + BodyHeight / 2;

	//левое колесо
	DrawWheel(leftWheelCenterX, wheelsCenterY);

	int rightWheelCenterX = X + BodyWidth / 4;

	//правое колесо
	DrawWheel(rightWheelCenterX, wheelsCenterY);
}

//Рисовать колесо с центром в точке (centerX, centerY)
void Bus::DrawWheel(int centerX, int centerY) {
	int wheelRadius = BodyWidth / 10;

	Ellipse(hdc, centerX - wheelRadius, centerY - wheelRadius, centerX + wheelRadius, centerY + wheelRadius);
}

//Обновление границ объекта
void Bus::UpdateHitbox() {
	rightBorder = X + BodyWidth / 2 + CabWidth;        //пересчитывем праую координату хитбокса
	leftBorder = X - (BodyWidth / 2);				   //пересчитывем левую координату хитбокса
	topBorder = Y - (BodyHeight / 2);				   //пересчитывем верхнюю координату хитбокса
	bottomBorder = Y + (BodyHeight / 2) + WheelRadius; //пересчитывем нижнюю координату хитбокса
}



/*----------------------------- Класс MiniBus ------------------------*/
MiniBus::MiniBus(int InitX, int InitY) : Bus(InitX, InitY) {
	//инициализация параметров автобуса
	BodyWidth = 120;
	BodyHeight = 40;
	WheelRadius = 12;
	CabWidth = 40;
	CabHeight = 20;

	//установка границ объекта
	UpdateHitbox();
	index = 0;		//присовение индекса соответсвующего объекту
	index = 1;  //индекс объекта
}


/*----------------------------- Класс ThreeWheelBus ------------------------*/
ThreeWheelBus::ThreeWheelBus(int InitX, int InitY) : Bus(InitX, InitY) { //конструктор
	BodyWidth = 180;
	BodyHeight = 60;
	WheelRadius = 18;
	CabWidth = 60;
	CabHeight = 30;

	UpdateHitbox();
	index = 2;  //индекс объекта
}

//рисовать 3 колeса
void ThreeWheelBus::DrawWheels(COLORREF color) { 
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	int leftWheelCenterX = X - BodyWidth / 3;
	int wheelsCenterY = Y + BodyHeight / 2;
	int rightWheelCenterX = X + BodyWidth / 3;

	//Левое колесо
	DrawWheel(leftWheelCenterX, wheelsCenterY);
	//Правое колесо
	DrawWheel(rightWheelCenterX, wheelsCenterY);
	//Центральное колесо
	DrawWheel(X, wheelsCenterY);

	DeleteObject(pen);
	DeleteObject(brush);
}

//Рисовать корпус трёхколёсого автобуса
void ThreeWheelBus::DrawBody(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Rectangle(hdc, X - BodyWidth / 2, Y - BodyHeight / 3, X, Y + BodyHeight / 2);
	Rectangle(hdc, X, Y - BodyHeight / 3, X + BodyWidth / 2, Y + BodyHeight / 2);
	DeleteObject(pen);
	DeleteObject(brush);
}


/*----------------------------- Класс ThreeWheelBus ------------------------*/
ThreeWheelBusWithTower::ThreeWheelBusWithTower(int InitX, int InitY) : ThreeWheelBus(InitX, InitY) { //конструктор
	BodyWidth = 180;
	BodyHeight = 60;
	WheelRadius = 18;
	CabWidth = 60;
	CabHeight = 30;

	UpdateHitbox();
	index = 4;  //индекс объекта
}

void ThreeWheelBusWithTower::UpdateHitbox() {
	rightBorder = X + BodyWidth / 2 + CabWidth;        //пересчитывем праую координату хитбокса
	leftBorder = X - (BodyWidth / 2);				   //пересчитывем левую координату хитбокса
	topBorder = Y - (BodyHeight);	      			   //пересчитывем верхнюю координату хитбокса
	bottomBorder = Y + (BodyHeight / 2) + WheelRadius; //пересчитывем нижнюю координату хитбокса
}

void ThreeWheelBusWithTower::Draw(COLORREF color) {
	DrawTower(RGB(255, 255, 0));				//башня
	DrawTube(RGB(255, 255, 0));     //труба
	DrawBody(RGB(255, 255, 0));     //корпус
	DrawCab(RGB(200, 200, 0));		//кабина
	DrawWheels(RGB(150, 150, 150)); //колёса
}

void ThreeWheelBusWithTower::DrawTower(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	int leftTowerBorderX = X - BodyWidth / 3;
	int rightTowerBorderX = X + BodyWidth / 3;

	Rectangle(hdc, leftTowerBorderX, Y - BodyHeight / 3, rightTowerBorderX, Y - BodyHeight);

	DeleteObject(pen);
	DeleteObject(brush);
}

void ThreeWheelBusWithTower::DrawTube(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Rectangle(hdc, X + BodyWidth / 2.8, Y - BodyHeight, X + BodyWidth / 2.2, Y - BodyHeight / 3);

	DeleteObject(pen);
	DeleteObject(brush);
}



/*----------------------------- Класс SquareBus ------------------------*/
//Конструктор
SquareBus::SquareBus(int InitX, int InitY) : Bus(InitX, InitY) {
	BodyWidth = 180;
	BodyHeight = 60;
	WheelRadius = 18;
	CabWidth = 60;
	CabHeight = 30;

	UpdateHitbox();
	index = 3; //установка индекса объекта
}

//Нарисовать корпус с трубой
void SquareBus::DrawBody(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	//Корпус
	Rectangle(hdc, X - BodyWidth / 2, Y - BodyHeight / 3, X + BodyWidth / 2, Y + BodyHeight / 2);
	//Труба
	Rectangle(hdc, X + BodyWidth / 5, Y - BodyHeight, X + BodyWidth / 3, Y - BodyHeight / 3);
	DeleteObject(pen);
	DeleteObject(brush);
}

//Нарисовать квадратное колесо
void SquareBus::DrawWheel(int centerX, int centerY) {
	int wheelRadius = BodyWidth / 10;

	Rectangle(hdc, centerX - wheelRadius, centerY - wheelRadius, centerX + wheelRadius, centerY + wheelRadius);
}

//Обновить границы с учётом трубы
void SquareBus::UpdateHitbox() {
	rightBorder = X + BodyWidth / 2 + CabWidth;  //пересчитывем праую координату хитбокса
	leftBorder = X - (BodyWidth / 2);   //пересчитывем левую координату хитбокса
	topBorder = Y - (BodyHeight / 3) - BodyHeight;          //пересчитывем верхнюю координату хитбокса
	bottomBorder = Y + (BodyHeight / 2) + WheelRadius;        //пересчитывем нижнюю координату хитбокса
}

/*--------------------- Класс Stone ------------------*/
//Конструктор 
Stone::Stone(int initX, int initY, int initHeight, int initWidth) : ABC_Figure(initX, initY) {
	BodyWidth = initWidth;
	BodyHeight = initHeight;
	index = 0;

	UpdateHitbox();
}

//Рисовать камень
void Stone::Draw(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	//рамка
	Rectangle(hdc, leftBorder, topBorder, rightBorder, bottomBorder);
	DeleteObject(pen);
	DeleteObject(brush);

	pen = CreatePen(PS_SOLID, 1, RGB(150, 150, 200));
	brush = CreateSolidBrush(RGB(150, 150, 200));
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	//камень
	Rectangle(hdc, leftBorder + BodyWidth / 5, topBorder + BodyHeight / 5, rightBorder - BodyWidth / 5, bottomBorder - BodyHeight / 5);
	DeleteObject(pen);
	DeleteObject(brush);
}

//Обновить границы
void Stone::UpdateHitbox() {
	rightBorder = X + BodyWidth / 2;
	leftBorder = X - BodyWidth / 2;
	bottomBorder = Y + BodyHeight / 2;
	topBorder = Y - BodyHeight / 2;
}

/*--------------------- Класс RepairKit ------------------*/
//конструктор
RepairKit::RepairKit(int InitX, int InitY, int InitHeight, int InitWidth) : ABC_Figure(InitX, InitY) {
	BodyHeight = InitHeight;
	BodyWidth = InitWidth;
	index = 1;

	//установление границ объекта
	UpdateHitbox();
}

//Рисовать ремонтный набор
void RepairKit::Draw(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	//рамка
	Rectangle(hdc, leftBorder, topBorder, rightBorder, bottomBorder);
	DeleteObject(pen);
	DeleteObject(brush);

	pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	brush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	//Плюсик
	Rectangle(hdc, leftBorder + BodyWidth / 5, topBorder + BodyWidth / 2.5, rightBorder - BodyWidth / 5, bottomBorder - BodyWidth / 2.5);
	Rectangle(hdc, leftBorder + BodyWidth / 2.5, topBorder + BodyWidth / 5, rightBorder - BodyWidth / 2.5, bottomBorder - BodyWidth / 5);

	DeleteObject(pen);
	DeleteObject(brush);
}

//Обновить границы
void RepairKit::UpdateHitbox() {
	rightBorder = X + BodyWidth / 2;
	leftBorder = X - BodyWidth / 2;
	bottomBorder = Y + BodyHeight / 2;
	topBorder = Y - BodyHeight / 2;
}

/*--------------------- Класс TrinityKit ------------------*/
TrinityKit::TrinityKit(int InitX, int InitY, int InitHeight, int InitWidth) : ABC_Figure(InitX, InitY) {
	BodyHeight = InitHeight;
	BodyWidth = InitWidth;
	index = 2;

	UpdateHitbox();
}

//Рисовать утроенный набор
void TrinityKit::Draw(COLORREF color) {
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	//рамка
	Rectangle(hdc, leftBorder, topBorder, rightBorder, bottomBorder);
	DeleteObject(pen);
	DeleteObject(brush);

	pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	brush = CreateSolidBrush(RGB(0, 255, 255));
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	//третье колесо
	Ellipse(hdc, leftBorder + BodyWidth / 3, topBorder + BodyHeight / 3, rightBorder - BodyWidth / 3, bottomBorder - BodyHeight / 3);
	DeleteObject(pen);
	DeleteObject(brush);
}

//Обновить границы
void TrinityKit::UpdateHitbox() {
	rightBorder = X + BodyWidth / 2;
	leftBorder = X - BodyWidth / 2;
	bottomBorder = Y + BodyHeight / 2;
	topBorder = Y - BodyHeight / 2;
}




/*-----------------------  Функции ----------------------------------*/
//Вывод всей иерархии объектов 
void PrintAllObjects() {
	//Рисуем все варианты автобусов 
	Bus A(100, 200);
	A.Show();
	MiniBus B(300, 200);
	B.Show();
	ThreeWheelBus C(500, 200);
	C.Show();
	SquareBus D(750, 200);
	D.Show();

	//Рисуем ремонтный набор
	RepairKit E(100, 400, 100, 100);
	E.Show();
	//Рисуем камень
	Stone F(300, 400, 100, 100);
	F.Show();
	//Рисуем тройной набор
	TrinityKit G(500, 400, 100, 100);
	G.Show();
}


//Симуляция столкновений
void CollisionSimulation(Bus* currBus, ABC_Figure* conflictObjects[], Bus** buses,
	int conflictObjectCount, int busCount, int** conflictResults) {
	int step = 10;
	currBus->Show();//показываем автобус

	//инициализация координат
	int x = currBus->GetX();
	int y = currBus->GetY();

	//бесконечный цикл буксировки
	while (true) {
		//выводим на экран все конфликтные объекты
		for (int i = 0; i < conflictObjectCount; i++) {
			conflictObjects[i]->Show();
		}


		//esc - выход
		if (KEY_DOWN(VK_ESCAPE)) {
			break;
		}

		//перемещение фигуры в зависимости от нажатой стрелки
		if (KEY_DOWN(VK_LEFT)) { //стрелка влево 37
			x -= step;				//изменение координаты
			currBus->MoveTo(x, y);  //перемещение автобуса
		}

		if (KEY_DOWN(VK_RIGHT)) //стрелка вправо  39
		{
			x += step;
			currBus->MoveTo(x, y);
		}

		if (KEY_DOWN(VK_UP)) //стрелка вверх  38
		{
			y -= step;
			currBus->MoveTo(x, y);
		}

		if (KEY_DOWN(VK_DOWN)) //стрелка вниз  40
		{
			y += step;
			currBus->MoveTo(x, y);
		}

		for (int i = 0; i < conflictObjectCount; i++) {
			//проверка на столкновение
			if (HadCollided(currBus, conflictObjects[i])) {
				//перемещаем в точку старта
				currBus->MoveTo(100, 250);
				//прячем
				currBus->Hide();
				//получаем координаты
				x = currBus->GetX();
				y = currBus->GetY();
				//меняем автобус
				currBus = buses[conflictResults[currBus->getIndex()][i]];
				//ставим в стартовое положение
				currBus->SetX(x);
				currBus->SetY(y);
				//показываем
				currBus->Show();
			}
		}
		Sleep(1);
	}
}


//Проверка столкновения 
bool HadCollided(Bus* currBus, ABC_Figure* conflictObject) {
	bool collisionFlag = false;  //флаг столкновения 
	collisionFlag =
		((currBus->getRightBorder() >= conflictObject->getLeftBorder() &&
			currBus->getRightBorder() < conflictObject->getRightBorder()) ||
			(currBus->getLeftBorder() <= conflictObject->getRightBorder() &&
				currBus->getLeftBorder() > conflictObject->getLeftBorder()) ||
			(currBus->getRightBorder() >= conflictObject->getRightBorder() &&
				currBus->getLeftBorder() <= conflictObject->getLeftBorder())) &&
		((currBus->getBottomBorder() >= conflictObject->getTopBorder() &&
			currBus->getBottomBorder() < conflictObject->getBottomBorder()) ||
			(currBus->getTopBorder() <= conflictObject->getBottomBorder() &&
				currBus->getTopBorder() > conflictObject->getTopBorder()) ||
			(currBus->getTopBorder() <= conflictObject->getTopBorder() &&
				currBus->getBottomBorder() >= conflictObject->getBottomBorder()));
	return collisionFlag;
}

