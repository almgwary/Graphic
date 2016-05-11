//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#include "Curve.h"

// compute the coefficients functions and store them in a 4D vector 

Vector4 HermiteCurve::GetHermiteCoeff(double x0,double s0,double x1,double s1)
{                         
        static double H[16]={2,1,-2,1,-3,-2,3,-1,0,1,0,0,1,0,0,0};
        static Matrix4 basis(H);
        Vector4 v(x0,s0,x1,s1);
        return basis*v;
}

//  numpoints is the number of points sampled from the curve function
//  used to compute the step by which  t  increases in the loop

void HermiteCurve::DrawHermiteCurve(HDC hdc,Vector2& P0,Vector2& T0,Vector2& P1,Vector2& T1 ,int numpoints, COLORREF color)
{
           Vector4 xcoeff=GetHermiteCoeff(P0.x,T0.x,P1.x,T1.x);   // p0 & p1 represent the position of the moving object
           Vector4 ycoeff=GetHermiteCoeff(P0.y,T0.y,P1.y,T1.y);
           if(numpoints<2)return;
           double dt=1.0/(numpoints-1);
           
           for(double t=0;t<=1;t+=dt)
           {
               Vector4 vt;
               vt[3]=1;
               for(int i=2;i>=0;i--)vt[i]=vt[i+1]*t;
               int x=(int)round(xcoeff.DotProduct(vt));
               int y=(int)round(ycoeff.DotProduct(vt));
               SetPixel(hdc,x,y,color);
           }
}

void HermiteCurve::DrawHermiteCurve(HDC hdc,int x1,int Y,int x2,int y2,int x3,int y3,int y4,int x4,int numpoints)
{
           Vector4 xcoeff=GetHermiteCoeff(x1,x2,x3,x4);
           Vector4 ycoeff=GetHermiteCoeff(Y,y2,y3,y4);
           if(numpoints<2)return;
           double dt=1.0/(numpoints-1);
           for(double t=0;t<=1;t+=dt)
           {
               Vector4 vt;
               vt[3]=1;
               for(int i=2;i>=0;i--)vt[i]=vt[i+1]*t;
               int x=(int)round(xcoeff.DotProduct(vt));
               int y=(int)round(ycoeff.DotProduct(vt));
               if(t==0)MoveToEx(hdc,x,y,NULL);else LineTo(hdc,x,y);
           }
}

//  use 4 points to draw  p0 , p1 , p2 , p3
//  start from p0 , end in p3
void HermiteCurve::DrawBezierCurve(HDC hdc,myPoint P0,myPoint P1,myPoint P2,myPoint P3,int numpoints)
{
           myPoint T0(3*(P1.x-P0.x),3*(P1.y-P0.y));
           myPoint T1(3*(P3.x-P2.x),3*(P3.y-P2.y));
           DrawHermiteCurve(hdc,P0.x,P0.y,T0.x,T0.y,P3.x,P3.y,T1.x,T1.y,numpoints);
}

void HermiteCurve::DrawCardinalSpline(HDC hdc,myPoint P[],int n,double c,int numpix)
{
           double c1=1-c;     // it take value from  0 to 1 
           myPoint T0((int)round(c1*(P[2].x-P[0].x)),(int)round(c1*(P[2].y-P[0].y)));
           for(int i=2;i<n-1;i++)
           {
                   myPoint T1((int)round(c1*(P[i+1].x-P[i-1].x)),(int)round(c1*(P[i+1].y-P[i-1].y)));
                   DrawHermiteCurve(hdc,P[i-1].x,P[i-1].y,T0.x,T0.y,P[i].x,P[i].y,T1.x,T1.y,numpix);
                   T0=T1;
           }
}
           
