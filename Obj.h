//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#pragma once
#include <windows.h>
#include <cmath>
#include <fstream>
#include "myLine.h"
#include "myCircle.h"
#include "Curve.h"
#include "Clipping.h"

using namespace std;

class Obj
{
      public:
             
             myPoint p1,p2;
             int x3,y3,x4,y4;
             int top,bottom,right,left,n;
             myPoint *Points;
             int shape;
             int algorithm;
             COLORREF color;
             myLine l;
             myCircle c;
             Clipping clip;
             HermiteCurve curve;
             Obj()
             {}
             Obj(myPoint, myPoint, int);
             void Draw(HDC);
             friend ifstream& operator>>(ifstream& f, Obj& o);
             friend ofstream& operator<<(ofstream& f, Obj& o);
};
           
