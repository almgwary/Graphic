//============================================================================
// Name        : Graphic
// Author      : Almgwary
// Date        : 2015
//============================================================================


#include <Windows.h>
#include "Obj.h"
#include <vector>
#define ID_FILE_EXIT 1
#define ID_FILE_Clear 2
#define ID_Line_Simple 3
#define ID_Line_DDA 4
#define ID_Line_Mid 5
#define ID_Circle_Simple 6
#define ID_Circle_Polar 7
#define ID_Circle_Mid 8
#define OPEN 9 
#define SAVE 10
#define ID_FILE_UNDO 11
#define Curve1 12
#define Curve2 13
#define Curve3 14
#define D_Window 15
#define P_Clip 16
#define L_Clip 17
#define PO_Clip 18

using namespace std;

vector<Obj>V;

//  Window Form to Draw my shapes on it 
LONG WINAPI WndProc(HWND,UINT,WPARAM,LPARAM);
int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow)
{
	WNDCLASSEX wcx;
	wcx.cbSize=sizeof(wcx);
	wcx.style=CS_HREDRAW|CS_VREDRAW;
	wcx.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	wcx.hCursor=LoadCursor(hInst,IDC_ARROW);
	wcx.hIcon=LoadIcon(hInst,IDI_WINLOGO);
	wcx.hIconSm=LoadIcon(hInst,IDI_WINLOGO);
	wcx.hInstance=hInst;
	wcx.lpfnWndProc=WndProc;
	wcx.lpszClassName="myclass";
	wcx.lpszMenuName=NULL;
	wcx.cbClsExtra=wcx.cbWndExtra=0;
	RegisterClassEx(&wcx);
	HWND hWnd=CreateWindowEx(0,wcx.lpszClassName,"Sheboo Graphics Pakage",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInst,NULL);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

int myPaint = 0;
int x1 = 0, Y = 0, xPos = 0, yPos = 0, x2 = 0, y2 = 0 ,  dx = 0, dy = 0, xp = 0,  yp =0,x3=0,y3=0,x4=0,y4=0;
myPoint p1,p2;
Obj o;
int alg = 0;
COLORREF color = RGB(250,0,250);
int shape = 0;
vector<myPoint>points;
bool OK = false;                            // to complete draw polyegon
int top = 0,bottom=0,xleft=0,xright=0;      // Cliping Prameter 

void Line(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
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


LONG WINAPI WndProc(HWND hWnd,UINT m,WPARAM wp,LPARAM lp)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch(m)
	{
	case WM_PAINT: 
		if((myPaint == 2 && shape != 3) || (myPaint == 4 && shape ==3) || (shape == 4 && alg == 3 && OK) || (myPaint == 2 && shape == 4 && alg != 3))
        {
                   hdc=BeginPaint(hWnd,&ps);
                   p1 = myPoint(x1,Y);
                   p2 = myPoint(x2,y2);
                   o = Obj(p1,p2,alg);
                   if(shape == 4)               // Clipping  Algorithm
                   {
                            o.left = xleft;
                            o.top = top;
                            o.right = xright;
                            o.bottom = bottom;
                            if(alg == 3)        //  Polygon Clipping
                            {
                                  
                                   o.n = points.size();
                                   o.Points = new myPoint[points.size()];
                                   for(int i=0;i<o.n;i++)
                                         {
                                         o.Points[i].x =  points[i].x;
                                         o.Points[i].y =  points[i].y;
                                         }
                                   points.clear();
                                   OK = false;
                            }
                   }
                   if(shape == 3)               //  curve Algorithm
                   {
                            o.x3 = x3;
                            o.y3=y3;
                            o.x4=x4;
                            o.y4=y4;
                   }
                   o.shape = shape;
                   o.color = color;
                   V.push_back(o);
                   o.Draw(hdc);
                   EndPaint(hWnd,&ps);
                   myPaint = 0;
        } 
		break;
         
	case WM_LBUTTONDOWN:
		xPos = LOWORD(lp);
		yPos = HIWORD(lp);
		if(myPaint == 0 && shape!=0)
		{
			x1 = xPos;
			Y = yPos;
			if(shape == 4 && alg == 3)     //  Polygon Clipping
            {
			myPoint point1 = myPoint(x1,Y);
            points.push_back(point1);
            }
            myPaint++;
            InvalidateRect(hWnd, NULL, false);
		}
		else if(myPaint == 1)
		{
			x2 = xPos;
			y2 = yPos;
			if(shape == 5)                //  draw window 
            {
                     top = Y > y2 ? y2 : Y;
                     bottom = Y < y2 ? y2 : Y;
                     xright = x1 > x2 ? x1 : x2;
                     xleft = x1 < x2 ? x1 : x2;
                     hdc=BeginPaint(hWnd,&ps);
                     Line(hdc,xleft,top,xright,top,color);
                     Line(hdc,xleft,top,xleft,bottom,color);
                     Line(hdc,xright,top,xright,bottom,color);
                     Line(hdc,xleft,bottom,xright,bottom,color);
                     EndPaint(hWnd,&ps);
                     myPaint = -1;
            }
			if(shape ==4 && alg==3)     //  Polygon Clipping
			{
                myPoint point2 = myPoint(x2,y2);
                hdc=BeginPaint(hWnd,&ps);
                Line(hdc,x1,Y,x2,y2,color);
                x1 = x2;
                Y=y2;
                EndPaint(hWnd,&ps);
                points.push_back(point2);
                myPaint = 0;
            }
			myPaint++;
            InvalidateRect(hWnd, NULL, false);
		}
		else if(myPaint == 2)
		{
			x3 = xPos;
			y3 = yPos;
			myPaint++;
		}
		else if(myPaint == 3)
		{
			x4 = xPos;
			y4 = yPos;
			myPaint++;
            InvalidateRect(hWnd, NULL, false);
   }
		break;
		
	case WM_RBUTTONDOWN:
         {
         OK = true;     //  to complete  Polygon Clipping points
         myPaint = 0;
         InvalidateRect(hWnd, NULL, true);
                     hdc=BeginPaint(hWnd,&ps);
                     for(int i=0; i<V.size();i++)
                             V[i].Draw(hdc);
                     Line(hdc,xleft,top,xright,top,color);
                     Line(hdc,xleft,top,xleft,bottom,color);
                     Line(hdc,xright,top,xright,bottom,color);
                     Line(hdc,xleft,bottom,xright,bottom,color);
                     EndPaint(hWnd,&ps);
		InvalidateRect(hWnd, NULL, false);
        }
         break;
         
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
		
	case WM_SIZE:
              hdc=BeginPaint(hWnd,&ps);
              for(int i=0; i<V.size();i++)
                      V[i].Draw(hdc);
              if(top != 0 && bottom != 0 && xright !=0 && xleft !=0)
              {                     
                     Line(hdc,xleft,top,xright,top,color);
                     Line(hdc,xleft,top,xleft,bottom,color);
                     Line(hdc,xright,top,xright,bottom,color);
                     Line(hdc,xleft,bottom,xright,bottom,color);
              }
              EndPaint(hWnd,&ps);
         break;
	
	case WM_CREATE:
         {
                   
                         
        HMENU hMenu, hSubMenu;
        
        hMenu = CreateMenu();

        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, OPEN, "Open");
        AppendMenu(hSubMenu, MF_STRING, SAVE, "Save");
        AppendMenu(hSubMenu, MF_STRING, ID_FILE_UNDO, "Undo");
        AppendMenu(hSubMenu, MF_STRING, ID_FILE_Clear, "Clear");
        AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "Exit");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "File");

        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, ID_Line_Simple, "Simple");
        AppendMenu(hSubMenu, MF_STRING, ID_Line_DDA, "DDA");
        AppendMenu(hSubMenu, MF_STRING, ID_Line_Mid, "Mid Point");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "Line");

        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, ID_Circle_Simple, "Simple");
        AppendMenu(hSubMenu, MF_STRING, ID_Circle_Polar, "Polar");
        AppendMenu(hSubMenu, MF_STRING, ID_Circle_Mid, "Mid Point");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "Circle");
        
        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, Curve1, "Hermite Curves");
        AppendMenu(hSubMenu, MF_STRING, Curve2, "Bezier Curves");
        AppendMenu(hSubMenu, MF_STRING, Curve3, "Cardinal splines");        
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "Curves");

        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, D_Window, "Draw Window");
        AppendMenu(hSubMenu, MF_STRING, P_Clip, "Point Clipping");
        AppendMenu(hSubMenu, MF_STRING, L_Clip, "Line Clipping");
        AppendMenu(hSubMenu, MF_STRING, PO_Clip, "Polygon Clipping");        
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "Clipping");
        
        SetMenu(hWnd, hMenu);

         }
         break;
         
	case WM_DESTROY:
		PostQuitMessage(0);
		
  	  case WM_COMMAND:
        switch(LOWORD(wp))
        {
            case ID_FILE_UNDO:   // to remove the last draw only 
                 InvalidateRect(hWnd, NULL, true);
                 V.pop_back();
                 hdc=BeginPaint(hWnd,&ps);
                 for(int i=0; i<V.size();i++)
                 V[i].Draw(hdc);
                 EndPaint(hWnd,&ps);
                 break;
                 
            case ID_FILE_EXIT:
                 PostMessage(hWnd, WM_CLOSE, 0, 0);
                 break;
            case ID_FILE_Clear:
                 InvalidateRect(hWnd, NULL, true);
                 V.clear();
                 break;
                 
            case OPEN:   // Open draw File 
                 {
                 OPENFILENAME openFileDialog;
                 char szFileName[MAX_PATH] = "";
                 ZeroMemory(&openFileDialog, sizeof(openFileDialog));    
                 openFileDialog.lStructSize= sizeof(openFileDialog);
                 openFileDialog.hwndOwner = hWnd;
                 openFileDialog.lpstrFilter = "Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
                 openFileDialog.lpstrFile = szFileName;
                 openFileDialog.nMaxFile = MAX_PATH;
                 openFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                 openFileDialog.lpstrDefExt = "txt";
                        
                 if(GetOpenFileName(&openFileDialog))
                 {
                 ifstream fin;            //  read from file 
                 fin.open(szFileName,ios::in);
                 if(fin.fail())break;
                 Obj ob;
                     while(!fin.eof())
                     {
                          fin >> ob;    // read object 
                          V.push_back(ob);
                     }
                     InvalidateRect(hWnd, NULL, false);
                     hdc=BeginPaint(hWnd,&ps);
                     for(int i=0; i<V.size();i++)
                             V[i].Draw(hdc);
                             EndPaint(hWnd,&ps);
                             fin.close();
                                                            
                 }
                 }
                 break;
            case SAVE:   // Save Draw in file
                 {
                        OPENFILENAME saveFileDialog;
                        char szSaveFileName[MAX_PATH] = "*.txt";
                        ZeroMemory(&saveFileDialog, sizeof(saveFileDialog));
                        saveFileDialog.lStructSize= sizeof(saveFileDialog);
                        saveFileDialog.hwndOwner = hWnd;
                        saveFileDialog.lpstrFilter = "Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
                        saveFileDialog.lpstrFile = szSaveFileName;
                        saveFileDialog.nMaxFile = MAX_PATH;
                        saveFileDialog.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT;
                        saveFileDialog.lpstrDefExt = "txt";
                        
                        if(GetSaveFileName(&saveFileDialog))
                        {
                        ofstream f;      // write on file
                        f.open(szSaveFileName,ios::out);
                        if(f.fail())break;
                        for(int i=0;i<V.size();i++)
                        {
                        f << V[i];
                        f << endl;
                        }
                                  f.close();
                        }
                 }
                 break;
            case ID_Line_Simple:
                 alg = 0;
                 shape = 1;
                 break;
            case ID_Line_DDA:
                 alg = 1;
                 shape = 1;
                 break;
            case ID_Line_Mid:
                 alg = 2;
                 shape = 1;
                 break;
            case ID_Circle_Simple:
                 alg = 0;
                 shape = 2;
                 break;
            case ID_Circle_Polar:
                 alg = 1;
                 shape = 2;
                 break;
            case ID_Circle_Mid:
                 alg = 2;
                 shape = 2;
                 break;
            case Curve1:
                 alg = 1;
                 shape = 3;
                 break;
            case Curve2:
                 alg = 2;
                 shape = 3;
                 break;
            case Curve3:
                 alg = 3;
                 shape = 3;
                 break;
            case D_Window:
                 shape = 5;
                 break;
            case P_Clip:
                 shape=4;
                 alg = 1;
                 break;
            case L_Clip:
                 shape = 4;
                 alg = 2;
                 break;
            case PO_Clip:
                 shape = 4;
                 alg = 3;
                 break; 
        }
		break;
		
	default: return DefWindowProc(hWnd,m,wp,lp);
	}
	return 0;
}
