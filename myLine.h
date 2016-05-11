//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#pragma once
#include "myPoint.h"
#include <windows.h>

class myLine
{
public:
       
	myPoint startPoint;
	myPoint endPoint;
	int drawingAlgorithm;
	COLORREF color;
	myLine();
	myLine(myPoint p1, myPoint p2, int alg);
	myLine(int x1, int y1, int x2, int y2);
	void drawLine(HDC hdc);
	void swap(int& x,int& y);
	int round(double x);
	void drawSimple(HDC hdc);
	void drawDDA(HDC hdc);
	void drawMid(HDC hdc);
};
