// Конструктор
Location::Location(int initX, int initY) {
    x = initX;
    y = initY;
} // Location::Location();

// Деструктор
Location::~Location() {

}

// Геттеры
int Location::getX() {
    return x;
}
int Location::getY() {
    return y;
}

// Сеттеры
void Location::setX(int newX) {
    x = newX;
}
void Location::setY(int newY) {
    y = newY;
}