#ifndef TCIRCLE_H
#define TCIRCLE_H

#include "TPoint.h"
class Circle : public Point {
	protected:
		 int Radius;
	public:
		Circle (int InitX, int InitY, int InitRadius);
		~Circle();
		void Show();
		void Hide();
		void Expand(int DeltaRad);
		void Reduce (int DeltaRad);
		// void MoveTo(NewX, int NeY);// ????? // Не переопределили благодаря this
};

#endif