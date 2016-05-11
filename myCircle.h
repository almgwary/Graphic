//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#pragma once
#include "myPoint.h"
#include <windows.h>
#include <cmath>

class myCircle
{
      public:
             
             myPoint Center;
             int R;
     	     COLORREF color;
             int drawingAlgorithm;
        	 myCircle();
        	 myCircle(myPoint p1, int r, int alg);
        	 myCircle(int x1, int y1, int r);
        	 int round(double x);
        	 void drawCircle(HDC hdc);
        	 void drawSimple(HDC hdc);
        	 void PolarAlgorithm(HDC hdc);
        	 void MidPoint(HDC hdc);
        	 void DrawPoints(HDC hdc, int xc, int yc, int x, int y);
};
