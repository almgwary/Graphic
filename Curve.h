//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#pragma once
#include <windows.h>
#include "myPoint.h"
#include <vector.h>
#include <iostream>
using namespace std;

struct Vector2    // vector with 2 points objects
{
       double x,y;
       Vector2(double a=0,double b=0)
       {
               x=a; y=b;
       }
};

class Vector4     // vector with 4 points objects
{
      double v[4];
      
      public:
             
      Vector4(double a=0,double b=0,double c=0,double d=0)
      {
              v[0]=a; v[1]=b; v[2]=c; v[3]=d;
      }
      
      Vector4(double a[])
      {
              memcpy(v,a,4*sizeof(double));   
      }
      
      double& operator[](int i)
      {
              return v[i];
      }

      double DotProduct(Vector4& b)
      {
             return v[0]*b[0]+v[1]*b[1]+v[2]*b[2]+v[3]*b[3];
      }
};

class Matrix4
{
      Vector4 M[4];
      public:
      Matrix4(double A[])
      {
               memcpy(M,A,16*sizeof(double));
      }
      
      Vector4& operator[](int i)
      {
               return M[i];
      }
      
      
      Vector4 operator*(Vector4& b)
      {
              Vector4 res;
              for(int i=0;i<4;i++)
                      for(int j=0;j<4;j++)
                              res[i]+=M[i][j]*b[j];
                              
              return res;
      }
      
      Vector4 multiply(Vector4& b)
      {
              Vector4 res;
              for(int i=0;i<4;i++)
                      for(int j=0;j<4;j++)
                              res[i]+=M[i][j]*b[j];
                              
              return res;
      }
};


class HermiteCurve
{
      public:
             
      COLORREF color;
      myPoint Points[4];
      
      HermiteCurve()
      {
      }
      
      Vector4 GetHermiteCoeff(double x0,double s0,double x1,double s1);
      void DrawHermiteCurve(HDC hdc,Vector2& P0,Vector2& T0,Vector2& P1,Vector2& T1 ,int numpoints, COLORREF color);
      void DrawHermiteCurve(HDC hdc,int x1,int Y,int x2,int y2,int x3,int y3,int y4,int x4,int numpoints);
      void DrawBezierCurve(HDC hdc,myPoint P0,myPoint P1,myPoint P2,myPoint P3,int numpoints);
      void DrawCardinalSpline(HDC hdc,myPoint P[],int n,double c,int numpix);
      void Draw(HDC hdc);
      int round(double x)
      {
          return (int)(x+0.5);
      }

};
