//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#include "Obj.h"

Obj::Obj(myPoint one,myPoint two, int alg)
{
                 p1 = one;
                 p2 = two;
                 algorithm = alg;
}
 
void Obj::Draw(HDC hdc)
{
     if(shape == 1)              //  Line Algorithms 
     {
              l = myLine(p1,p2,algorithm);
              l.color = color;
              l.drawLine(hdc);
     }
     else if(shape == 2)       //  Circle Algorithms 
     {
          int dx = p1.x-p2.x;
          int dy = p1.y-p2.y;
          int r = (int)sqrt(dx*dx + dy*dy);
          c = myCircle(p1,r,algorithm);
          c.color = color;
          c.drawCircle(hdc);
     }
        else if(shape == 3)    //  Curve Algorithms 
     {
          curve.color = color;
          curve.Points[0] = p1;
          curve.Points[1] = p2;
          curve.Points[2] = myPoint(x3,y3);
          curve.Points[3] = myPoint(x4,y4);
          if(algorithm == 1)
                 curve.DrawHermiteCurve(hdc,p1.x,p1.y,p2.x,p2.y,x3,y3,x4,y4,100);
          if(algorithm == 2)
                 curve.DrawBezierCurve(hdc,curve.Points[0],curve.Points[1],curve.Points[2],curve.Points[3],100);
          if(algorithm == 3)
                 curve.DrawCardinalSpline(hdc,curve.Points,4,0.01,100);
     }
     else if(shape == 4)      //  Clipping Algorithms 
     {
          if(algorithm == 1)
                       clip.PointClipping(hdc,p1.x,p1.y,left,top,right,bottom,color);
          if(algorithm == 2)
                       clip.LineClipping(hdc,p1.x,p1.y,p2.x,p2.y,left,top,right,bottom);
          if(algorithm == 3)
                      {clip.PolygonClipping(hdc,Points,n,left,top,right,bottom);}
     }
}

ifstream& operator>>(ifstream& f, Obj& o)    //  Write on file 
{
         f  >> o.p1.x >> o.p1.y >> o.p2.x >> o.p2.y >> o.x3 >> o.y3 >> o.x4 >> o.y4 >> o.shape >> o.algorithm >> o.color;
         return f;
}
ofstream& operator<<(ofstream& f, Obj& o)   //  Read From file 
{
         f  << o.p1.x << " " << o.p1.y << " " << o.p2.x << " " << o.p2.y << " " << o.x3 << " " << o.y3 << " " << o.x4 << " " << o.y4 << " " << o.shape << " " << o.algorithm << " " << o.color;
         return f;
}
