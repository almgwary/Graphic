//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================
#include "Clipping.h"

int round(double x)
{
	return (int)(x+0.5);
}

void dLine(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)    // draw line
{     
	int dx=xe-xs;
	int dy=ye-ys;
	
	if(abs(dy)<=abs(dx))
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

void Clipping::PointClipping(HDC hdc,int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)  // draw clipping point
{
     if(x >= xleft && x <= xright && y >= ytop && y <= ybottom)
                 SetPixel(hdc,x,y,color);
}

OutCode Clipping::GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)  // gets the side code
{
        OutCode out;
        out.All=0; 
        if(x<xleft)out.left=1;           //  point is outside left  the window 
        else if(x>xright)out.right=1;    //  point is outside right the window
        if(y<ytop)out.top=1;             //  point is outside above the window
        else if(y>ybottom)out.bottom=1;  //  point is outside under the window
        return out;
}

void Clipping::VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)   //  vertical intersection points with edges
{
     *xi=x;
     *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void Clipping::HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)   // horizontal intersection points with edges
{
     *yi=y;
     *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}

void Clipping::LineClipping(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)  // draw clipping line
{
     double x1=xs,y1=ys,x2=xe,y2=ye;
     OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
     OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
     while( (out1.All || out2.All) && !(out1.All & out2.All))
     {
            double xi,yi;
            if(out1.All)
            {
                        if(out1.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
                        else if(out1.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
                        else if(out1.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
                        else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
                        x1=xi;
                        y1=yi;
                        out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
                        } else
                        {
                              if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
                              else if(out2.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
                              else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
                              else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
                              x2=xi;
                              y2=yi;
                              out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
                        }
            }
     if(!out1.All && !out2.All)
     {
                  MoveToEx(hdc,round(x1),round(y1),NULL);
                  LineTo(hdc,round(x2),round(y2));

                  dLine(hdc,round(x1),round(y1),round(x2),round(y2),RGB(0,0,0));
     }
}

VertexList Clipping::ClipWithEdge(VertexList p,int edge,IsInFunc In,IntersectFunc Intersect)
{
           VertexList OutList;
           Vertex v1=p[p.size()-1];
           bool v1_in=In(v1,edge);
           for(int i=0;i<(int)p.size();i++)
           {
                   Vertex v2=p[i];
                   bool v2_in=In(v2,edge);
                   if(!v1_in && v2_in)
                   {
                             OutList.push_back(Intersect(v1,v2,edge));
                             OutList.push_back(v2);
                   }else if(v1_in && v2_in) OutList.push_back(v2);
                   else if(v1_in) OutList.push_back(Intersect(v1,v2,edge));
                   v1 = v2;
                   v1_in = v2_in;
           }
           return OutList;
}

bool InLeft(Vertex& v,int edge)
{
     return v.x >= edge;
}
bool InRight(Vertex& v,int edge)
{
     return v.x <= edge;
}
bool InTop(Vertex& v,int edge)
{
     return v.y >= edge;
}
bool InBottom(Vertex& v,int edge)
{
     return v.y <= edge;
}

Vertex vIntersect(Vertex& v1,Vertex& v2,int xedge)
{
       Vertex res; 
       res.x = xedge;
       res.y = v1.y+(xedge-v1.x)*(v2.y-v1.y)/(v2.x-v1.x);
       return res;
}
Vertex hIntersect(Vertex& v1,Vertex& v2,int yedge)
{
       Vertex res;
       res.y = yedge;
       res.x = v1.x+(yedge-v1.y)*(v2.x-v1.x)/(v2.y-v1.y);
       return res;
}

void Clipping::PolygonClipping(HDC hdc,myPoint *p,int n,int xleft,int ytop,int xright,int ybottom)
{
     VertexList vlist;
     for(int i=0;i<n;i++)vlist.push_back(Vertex(p[i].x,p[i].y));
     vlist = ClipWithEdge(vlist,xleft,InLeft,vIntersect);
     vlist = ClipWithEdge(vlist,ytop,InTop,hIntersect);
     vlist = ClipWithEdge(vlist,xright,InRight,vIntersect);
     vlist = ClipWithEdge(vlist,ybottom,InBottom,hIntersect); 
     Vertex v1 = vlist[vlist.size()-1];
     for(int i=0;i<(int)vlist.size();i++)
     { 
             Vertex v2 = vlist[i];
             dLine(hdc,round(v1.x),round(v1.y),round(v2.x),round(v2.y),RGB(0,0,0));
            
             v1=v2;
     }
}
