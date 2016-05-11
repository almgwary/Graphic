//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#include "myLine.h"
#include "myPoint.h"
#include <windows.h> 

myLine::myLine()
{
	drawingAlgorithm = 0;
}
myLine::myLine(myPoint p1, myPoint p2, int alg)
{
	startPoint = p1;
	endPoint = p2;
	drawingAlgorithm = alg;
}
myLine::myLine(int x1, int y1, int x2, int y2)
{
	startPoint = myPoint(x1,y1);
	endPoint = myPoint(x2,y2);
	drawingAlgorithm = 0;
}
void myLine::drawLine(HDC hdc)
{
	if(drawingAlgorithm == 0)
		drawSimple(hdc);
	else if(drawingAlgorithm == 1)
		drawDDA(hdc);
	else if(drawingAlgorithm == 2)
		drawMid(hdc);
}
void myLine::swap(int& x,int& y)
{
	int tmp=x;
	x=y;
	y=tmp;
}

/***************************************************************/
int myLine::round(double x)
{
	return (int)(x+0.5);
}

void myLine::drawSimple(HDC hdc)
{
	int xs = startPoint.x, xe = endPoint.x, ys = startPoint.y, ye = endPoint.y;

	int dx=xe-xs;
	int dy=ye-ys;
	 
	if(dx == 0 && dy == 0)
	{
		SetPixel(hdc,xs,ys,color);
	}

	else if(dx == 0)
	{
		if(ys>ye)
		{
			swap(ys,ye);
		}
		for(int y=ys;y<=ye;y++)
		{
			SetPixel(hdc,xs,y,color);
		}
	}

	else if(dy == 0)
	{
		if(xs>xe)
		{
			swap(xs,xe);
		}
		for(int x=xs;x<=xe;x++)
		{
			SetPixel(hdc,x,ys,color);
		}
	}
	
	else if(abs(dy)<=abs(dx))
	{
		double slope=(double)dy/dx;
		if(xs>xe)
		{
			swap(xs,xe);
			swap(ys,ye);
		}
		for(int x=xs;x<=xe;x++)
		{
			int y=round(ys+(x-xs)*slope);
			SetPixel(hdc,x,y,color);
		}
	} 
	
	else
	{
		double islope=(double)dx/dy;
		if(ys>ye)
		{
			swap(xs,xe);
			swap(ys,ye);
		}
		for(int y=ys;y<=ye;y++)
		{
			int x=round(xs+(y-ys)*islope);
			SetPixel(hdc,x,y,color);
		}
	}
}
void myLine::drawDDA(HDC hdc)
{
     int xs = startPoint.x, xe = endPoint.x, ys = startPoint.y, ye = endPoint.y;
     
    int dx=xe-xs;
    int dy=ye-ys;
    SetPixel(hdc,xs,ys,color);
    if(abs(dx)>=abs(dy))
    {
                        int x=xs,xinc= dx>0?1:-1;
                        double y=ys,yinc=(double)dy/dx*xinc;
                        while(x!=xe)
                        {
                                    x+=xinc;
                                    y+=yinc;
                                    SetPixel(hdc,x,round(y),color);
                        }
    }
    else
    {
        int y=ys,yinc= dy>0?1:-1;
        double x=xs,xinc=(double)dx/dy*yinc;
        while(y!=ye)
        {
                    x+=xinc;
                    y+=yinc;
                    SetPixel(hdc,round(x),y,color);
        }
    }
}
void myLine::drawMid(HDC hdc)
{
     int xs = startPoint.x, xe = endPoint.x, ys = startPoint.y, ye = endPoint.y;
     
     int dx = xe - xs;
     int dy = ye - ys;
     int d = 0;
     int change1 = 0, change2 = 0, incx = 0, incy = 0;
     int x = 0;
     int y = 0;
     
     if(abs(dy) <= abs(dx))
     {
                if(dx < 0)
                {
                      swap(xs,xe);
                      swap(ys,ye);
                      dx = -dx;
                      dy = -dy;
                }
                d = dx - (2 * abs(dy));
                change1 = 2 * (dx - abs(dy));
                change2 = -2 * (abs(dy));
                incy = dy >= 0 ? 1 : -1;
                x = xs;
                y = ys;
                SetPixel(hdc,x,y,color);
                while(x < xe)
                {
                        if(d < 0)
                        {
                             d = d + change1;
                             y = y + incy;
                        }
                        else
                            d = d + change2;
                        x++;
                        SetPixel(hdc,x,y,color);
                } 
     }
     else
     {
                if(dy < 0)
                {
                      swap(xs,xe);
                      swap(ys,ye);
                      dx = -dx;
                      dy = -dy;
                }
                d = (2 * abs(dx)) - dy;
                change1 = 2 * (abs(dx) - dy);
                change2 = 2 * abs(dx);
                incx = dx > 0 ? 1 : -1;
                x = xs;
                y = ys;
                SetPixel(hdc,x,y,color);
                while(y < ye)
                {
                        if(d > 0)
                        {
                             d = d + change1;
                             x = x + incx;
                        }
                        else
                            d = d + change2;
                        y++;
                        SetPixel(hdc,x,y,color);
                }          
     }

}
