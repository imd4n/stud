#include "TCircle.h"
#include "windows.h"

extern HDC hdc;

// Реализация методов в классе Circle
Circle::Circle(int InitX, int InitY, int InitRadius):Point(InitX, InitY)
{
	Radius = InitRadius;
}

Circle::~Circle() {}

void Circle::Show() {
	Visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0,0,255));
	SelectObject(hdc, Pen);
	Ellipse(hdc, x-Radius, y-Radius, x+Radius, y+Radius);
	DeleteObject(Pen);
} // Circle::Sow()

void Circle::Hide() {
	Visible = false;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
	SelectObject(hdc, Pen);
	Ellipse(hdc, x=Radius, y-Radius, x+Radius, y+Radius);
	DeleteObject(Pen);
} // Circle::Sow()

void Circle::Expand(int DeltaRad) {
 Hide();
 Radius+=DeltaRad;
 if (Radius<2) {
	 Radius = 2;
	 Show();
	}
} // Circle::Expand()

void Circle::Reduce(int DeltaRad) {
	Expand(-DeltaRad);
} // Circle::Reduce()

