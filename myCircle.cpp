//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#include "myCircle.h"

myCircle::myCircle()
{
                             Center = myPoint(0,0);
                             R = 0;
                             drawingAlgorithm = 0;
}
myCircle::myCircle(myPoint p1, int r, int alg)
{
     Center = p1;
     R = r;
     drawingAlgorithm = alg;
}
myCircle::myCircle(int x1, int y1, int r)
{
     Center = myPoint(x1,y1);
     R = r;
}
void myCircle::drawCircle(HDC hdc)
{
     switch(drawingAlgorithm)
     {
                             case 0:
                                  drawSimple(hdc);
                                  break;
                             case 1:
                                  PolarAlgorithm(hdc);
                                  break;
                             case 2:
                                  MidPoint(hdc);
                                  break;
                             default:
                                     break;
     }
}
void myCircle::drawSimple(HDC hdc)
{
     int x = 0, y = R;
     DrawPoints(hdc, Center.x,Center.y,x,y);
     
     while(x < y)
     {
               x++;
               y = (int)(sqrt(R*R - x*x));
               DrawPoints(hdc,Center.x,Center.y,x,y);
     }
}

void myCircle::DrawPoints(HDC hdc, int xc, int yc, int x, int y)
{
     SetPixel(hdc, xc+x,yc+y,color);
     SetPixel(hdc, xc-x,yc+y,color);
     SetPixel(hdc, xc-x,yc-y,color);
     SetPixel(hdc, xc+x,yc-y,color);
     SetPixel(hdc, xc+y,yc+x,color);
     SetPixel(hdc, xc-y,yc+x,color);
     SetPixel(hdc, xc-y,yc-x,color);
     SetPixel(hdc, xc+y,yc-x,color);
}

int myCircle::round(double x)
{
	return (int)(x+0.5);
}

void myCircle::PolarAlgorithm(HDC hdc)
{
    double x = R, y = 0;
    double dtheta = 1.0/R;
    double cdtheta = cos(dtheta), sdtheta = sin(dtheta);
    DrawPoints(hdc,Center.x,Center.y,R,0);
    while(x>y)
    {
              double x1 = x*cdtheta - y*sdtheta;
              y = x*sdtheta + y*cdtheta;
              x = x1;
              DrawPoints(hdc,Center.x,Center.y,round(x),round(y));
    }     
}

void myCircle::MidPoint(HDC hdc)
{
     int x = 0, y = R, d = 1-R, dch1 = 3, dch2 = (5-2*R);
     DrawPoints(hdc,Center.x,Center.y,x,y);
     
     while(x<y)
     {
               if(d<0)
               {
                      d += dch1;
                      dch2 += 2;
               }
               else
               {
                   d += dch2;
                   y--;
                   dch2 += 4;
               }
               dch1 += 2;
               x++;
               DrawPoints(hdc,Center.x,Center.y,x,y);
     }
}
