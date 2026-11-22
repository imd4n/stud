class Location
{
//private:
protected: // Доступно всем потомкам
    // Координаты
    int x;
    int y;
public:
    Location(int InitX, int InitY); // Конструктор
    ~Location(); // Деструктор (очистка памяти)

    // Получение
    int getX();
    int getY();
    // Изменение значений полей
    void setX(int newX);
    void setY(int newY);
}; // class Location

// Потомок "точка"
class Point:public Location
{
protected:
    // Задаем только одну переменную, так как остальные наследуются
    bool Visible;
public:
    Point (int initX, int initY); // Конструктор
    ~Point(); // Деструктор

    // Функции для работы с точкой
    virtual void Show(); // Показать
    virtual void Hide(); // Спрятать

    void moveTo(int newX, int newY); // Перемещение

    // Дописать сет и гет
}; // class Point

class Line:public Point
{
private:
    double angle;
    double length;
public:
    Line(int initX1, int initY1, int initX2, int initY2); // Конструктор
    Line(int initX, int initY, double _angle, double _length);
    ~Line(); // Деструктор

    // Функции для работы с точкой
    void Show(); // Показать
    void Hide(); // Спрятать

    void moveTo(int newX, int newY); // Перемещение

    // TODO: Сделать изменение угла, длинны, а также перемещение по двум точкам
    // Дописать сет и гет
}; // class Point

class Circle : public Point {
	protected:
		 int Radius;
	public:
		Circle (int InitX, int InitY, int InitRadius);
		~Circle();
		virtual void Show() override;
		virtual void Hide() override;
		void Expand(int DeltaRad);
		void Reduce (int DeltaRad);
		//void moveTo(int newX, int newY);// ?????
};

class Square:public Point {
private:
    int width;
    int height;
public:
    Square(int initX, int initY, int _width, int _height);
    ~Square();

    int getWidth();
    int getHeight();

    void Show();
    void Hide();

    void moveTo(int newX, int newY);
};

class Plane:public Point {
private:
    Square *CollisionObj;
    Line **lineArr;
public: Plane(int initX, int initY);
    ~Plane();

    void Show();
    void Hide();

    void moveTo (int newX, int newY);
    void resize();

    Square* getCollisionObj();
};
/*
// Обновленный класс самолета, содержит в себе только минимально необходимое
// Не содержит функций отрисовки и тому подобного, просто базовый костяк функций,
// которые есть в самолете
class Plane:public Point {
private:
    // Рамка для определения коллизии
    Square *CollisionObj;
    // Массивы обьектов, которые в последствии будут отрисовывать обьект
    Line   **lineArr;
    Circle **circleArr;
    Square **squareArr;
public:
    // Конструктор класса, для себя определяет его местоположение
    Plane(int initX, int initY);

    // Функция отображения
    virtual void setVisible(bool _visible);

    // Функция перемещения
    virtual void moveTo(int newX, int newY);
    // Фунция относительного перемещения
    virtual void deltaMoveTo (int deltaX, int deltaY);

    // Геттеры
    virtual Square *getCollisionObj();
    virtual Line **getLineArr();
    virtual Circle **getCircleArr();
    virtual Square **getSquareArr();

    // Сеттеры
    virtual void setCollisionObj();
    virtual void setLineArr();
    virtual void setCircleArr();
    virtual void setSquareArr();
};

// Пассажирский самолет
class PassengersPlane:public Plane {
public:
    // Необходимо только переопределить единственный оператор отрисовки, остальные функции без проблем можно взять из предыдущего класса
    void drawObject();
};
*/