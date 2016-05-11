//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#include <windows.h>
#include "myPoint.h"
#include <vector>
using namespace std;

struct Vertex
{
       double x,y;
       Vertex(int x1=0,int y1=0)
       {
                  x = x1;
                  y = y1;
       }
};

typedef vector<Vertex> VertexList;
typedef bool (*IsInFunc)(Vertex& v,int edge);
typedef Vertex (*IntersectFunc)(Vertex& v1,Vertex& v2,int edge);

union OutCode
{
      unsigned All:4;
      struct{unsigned left:1,top:1,right:1,bottom:1;};
};

class Clipping
{
      public:
             
             void PointClipping(HDC,int,int,int,int,int,int,COLORREF);
             OutCode GetOutCode(double,double,int,int,int,int);
             void LineClipping(HDC,int,int,int,int,int,int,int,int);
             VertexList ClipWithEdge(VertexList,int,IsInFunc,IntersectFunc);
             void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi);
             void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi); 
             void PolygonClipping(HDC,myPoint*,int,int,int,int,int);
             void DrawPoints(HDC,int,int,int,int,int,int,int,int,COLORREF);
            
};
