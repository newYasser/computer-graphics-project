#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

#define LINE_DDA 1
#define LINE_MID_POINT 2
#define LINE_PARAMETRIC 3
#define CIRCLE_DIRECT 4
#define CIRCLE_POLAR 5
#define CIRCLE_ITERATIVE_POLAR 7
#define CIRCLE_MIDPOINT 8
#define CIRCLE_MODIFIED_MIDPOINT 9
#define FILLING_CIRCLE_WITH_LINES 10
#define FILLING_CIRCLE_WITH_CIRCLES 11
#define FILLING_SQUARE_WITH_HERMIT_CURVE 12
#define FILLING_RECTANGLE_WITH_BEZIER_CURVE 13
#define FILLING_CONVEX 14
#define FILLING_NON_CONVEX 15
#define FLOOD_FILL_RECURSIVE 16
#define FLOOD_FILL_NON_RECURSIVE 17
#define CARDINAL_SPLINE_CURVE 18
#define ELLIPSE_DIRECT 19
#define ELLIPSE_POLAR 20
#define ELLIPSE_MIDPOINT 21
#define CLIPPING_USING_RECTANGLE_POINT 22
#define CLIPPING_USING_RECTANGLE_LINE 23
#define CLIPPING_USING_RECTANGLE_POLYGON 24
#define CLIPPING_USING_SQUARE_LINE 25
#define CLIPPING_USING_SQUARE_POLYGON 26

struct point
{
    int x, y;
};

union OutCode
{
    unsigned All : 4;
    struct
    {
        unsigned left : 1, top : 1, right : 1, bottom : 1;
    };
};
HMENU hMenu;
LRESULT CALLBACK WindowProcedure(HWND,UINT,WPARAM,LPARAM);
void AddMenus(HWND);
int round(double);
void drawLine_directMethod(HDC ,point,point, COLORREF);
//void drawRectangle(HDC hdc);
OutCode GetOutCode(point , int , int , int , int );
void VIntersect(point, point, int , int*, int*);
void HIntersect(point , point , int, int*, int*);
void CohenSuth(HDC , point , point , int , int , int , int , COLORREF );
void DrawPolygon(HDC, vector<point> , COLORREF);
void MidPointLine(HDC, point, point , COLORREF );




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,LPSTR args,int ncmdshow)
{
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hInstance = hInst;
    wc.hInstance = hInst;
    wc.lpszClassName = (L"WindowClass");
    wc.lpfnWndProc = WindowProcedure;

    if(!RegisterClassW(&wc)){
        cout << "Register Failed" <<endl;
        return -1;
    }

    CreateWindowW((L"WindowClass"), (L"Graphics Project"),
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg  = {0};
    while(GetMessage(&msg,NULL,NULL,NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
    static int x, y;
    HDC hdc = GetDC(hWnd) ;
   static  vector<point>points;
   static  int clicks = 2;
   static int idx = 0;
   switch(msg)
   {
       case WM_LBUTTONDOWN:
           point p;
           p.x = LOWORD(lp);
           p.y = HIWORD(lp);
           points.push_back(p);
           SetPixel(hdc,x,y,RGB(255,0,0));
           break;
       /* when something is clicked anything here will be performed
        * the w parameter determine which item is clicked
        */
       case WM_COMMAND:
           switch (wp)
           {

               case CLIPPING_USING_RECTANGLE_LINE:
                  // drawRectangle(hdc);
                   cout << points[0].x << ' ' << points[0].y << endl;
                   cout << points[1].x << ' ' << points[1].y << endl;
                  // CohenSuth(hdc,points[idx - 1],points[idx - 2],100,401,501,200,RGB(0,0,255));
                   break;
               case CLIPPING_USING_RECTANGLE_POLYGON:
                   DrawPolygon(hdc,points, RGB(255,0,0));
                   break;
           }
           break;
       case WM_CREATE:
           AddMenus(hWnd);
          // AddControls(hWnd);
           break;
       case WM_DESTROY:
           PostQuitMessage(0);
           break;
       case WM_PAINT:
       {
           PAINTSTRUCT ps;
           HDC hdc = BeginPaint(hWnd, &ps);
           HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
           FillRect(hdc, &ps.rcPaint, hBrush);

           EndPaint(hWnd, &ps);
       }
           break;
       default:
           return DefWindowProc(hWnd,msg,wp,lp);

   }

}

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();
    HMENU hLineMenu = CreateMenu();
    HMENU hCircleMenu = CreateMenu();
    HMENU hFillingMenu = CreateMenu();
    HMENU hEllipseMenu = CreateMenu();
    HMENU hClippingMenu = CreateMenu();
    HMENU hFloodFillMenu = CreateMenu();
    HMENU hCurveMenu = CreateMenu();
    HMENU hClippingSquareMenu = CreateMenu();
    HMENU hClippingRectangleMenu = CreateMenu();



    AppendMenu(hLineMenu,MF_STRING,LINE_DDA,"DDA");
    AppendMenu(hLineMenu,MF_STRING,LINE_MID_POINT,"Mid Point");
    AppendMenu(hLineMenu,MF_STRING,LINE_PARAMETRIC,"Parametric");

    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_DIRECT,"Direct");
    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_POLAR,"Polar");
    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_ITERATIVE_POLAR,"Iterative Polar");
    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_MIDPOINT,"Mid Point");
    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_MODIFIED_MIDPOINT,"Modified Midpoint");

    AppendMenu(hFillingMenu,MF_STRING,FILLING_CIRCLE_WITH_LINES,"Circle with lines");
    AppendMenu(hFillingMenu,MF_STRING,FILLING_CIRCLE_WITH_CIRCLES,"Circle with other circles");
    AppendMenu(hFillingMenu,MF_STRING,FILLING_SQUARE_WITH_HERMIT_CURVE,"Square with Hermit");
    AppendMenu(hFillingMenu,MF_STRING,FILLING_RECTANGLE_WITH_BEZIER_CURVE,"Rectangle With Bezier");
    AppendMenu(hFillingMenu,MF_STRING,FILLING_CONVEX,"Convex");
    AppendMenu(hFillingMenu,MF_STRING,FILLING_NON_CONVEX,"Non Convex");
    AppendMenu(hFloodFillMenu,MF_STRING,FLOOD_FILL_RECURSIVE,"Recursive");
    AppendMenu(hFloodFillMenu,MF_STRING,FLOOD_FILL_NON_RECURSIVE,"Non Recursive");

    AppendMenu(hFillingMenu,MF_POPUP,(UINT_PTR)hFloodFillMenu,"Flood Fill");

    AppendMenu(hCurveMenu,MF_STRING,CARDINAL_SPLINE_CURVE,"Cardinal Spline");


    AppendMenu(hEllipseMenu,MF_STRING,ELLIPSE_DIRECT,"Direct");
    AppendMenu(hEllipseMenu,MF_STRING,ELLIPSE_POLAR,"Polar");
    AppendMenu(hEllipseMenu,MF_STRING,ELLIPSE_MIDPOINT,"Mid Point");

    AppendMenu(hClippingRectangleMenu,MF_STRING,CLIPPING_USING_RECTANGLE_POINT,"Point");
    AppendMenu(hClippingRectangleMenu,MF_STRING,CLIPPING_USING_RECTANGLE_LINE,"Line");
    AppendMenu(hClippingRectangleMenu,MF_STRING,CLIPPING_USING_RECTANGLE_POLYGON,"Polygon");
    AppendMenu(hClippingSquareMenu,MF_STRING,CLIPPING_USING_SQUARE_LINE,"Line");
    AppendMenu(hClippingSquareMenu,MF_STRING,CLIPPING_USING_SQUARE_POLYGON,"Polygon");


    AppendMenu(hClippingMenu,MF_POPUP,(UINT_PTR)hClippingSquareMenu,"Square");
    AppendMenu(hClippingMenu,MF_POPUP,(UINT_PTR)hClippingRectangleMenu,"Rectangle");




    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hLineMenu,"Line");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hCircleMenu,"Circle");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFillingMenu,"Filling");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hCurveMenu,"Curve");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hEllipseMenu,"Ellipse");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hClippingMenu,"Clipping");

    SetMenu(hWnd,hMenu);
}


// starting point of execution

int round(double point){
    // check the nearest integer point ( to determine whether to round down or up )
    if(point - (int)point > 0.5)
        return (int)point + 1 ;
    else
        return (int)point ;
}
int max(int x , int y){
    return (x>y ? x : y) ;
}

void drawLine_directMethod(HDC hdc , point p1,point p2, COLORREF color){
    int x1 = p1.x,y1 = p1.y,x2 = p2.x,y2 = p2.y;
    int dy = y2-y1 ;
    int dx = x2-x1 ;
    double slope = dy/dx ;
    // error if dy = 0 ;
    double slopeInverse = dx/dy ;
    // check the spread in x and y to determine which coordinate will be the independent value , and make the other dependent on it
    // if x is independent ==> y = y1 + (x-x1) * slope
    // if y is independent ==> x = x1 + (y-y1) * (1/slope)
    // abs( ) : values may be -ve
    if(abs(dx)>abs(dy)){
        // x is independent
        // swap to always make x2 > x1 , so we always iterate from x1 to x2 in the loop
        if(x1>x2){
            swap(x1,x2) ;
            swap(y1,y2) ;
        }
        for(int x = x1 ; x<=x2 ; ++x){
            double y = y1 + (x-x1)*slope ;
            y = round(y) ;
            SetPixel(hdc,x,y,color) ;
        }
    }
    else{
        // y is independent
        if(y1>y2){
            swap(x1,x2) ;
            swap(y1,y2) ;
        }
        for(int y=y1 ; y<=y2 ; ++y){
            double x = x1 + (y-y1)*slopeInverse ;
            x = round(x) ;
            SetPixel(hdc,x,y,color) ;
        }
    }

}
/*
void drawRectangle(HDC hdc){
    int x1 = 100,x2 = 101,x3 = 500,x4 = 501,y1 = 200,y2 = 400,y3 = 201,y4 = 401;
    drawLine_directMethod(hdc,x1,y1,x2,y2,RGB(0,0,255)) ;
    drawLine_directMethod(hdc,x3,y3,x4,y4,RGB(0,0,255)) ;
    drawLine_directMethod(hdc,x1,y1,x3,y3,RGB(0,0,255)) ;
    drawLine_directMethod(hdc,x2,y2,x4,y4,RGB(0,0,255)) ;
}
 */


OutCode GetOutCode(point p1, int xleft, int ytop, int xright, int ybottom)
{
    OutCode out;
    out.All = 0;
    if (p1.x < xleft)
        out.left = 1;
    else if (p1.x > xright)
        out.right = 1;
    if (p1.y < ytop)
        out.top = 1;
    else if (p1.y > ybottom)
        out.bottom = 1;
    return out;
}

void VIntersect(point p1, point p2, int x, int *xi, int *yi)
{
    *xi = x;
    *yi = p1.y + (x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
}
void HIntersect(point p1, point p2, int y, int *xi, int *yi)
{
    *yi = y;
    *xi = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
}
void CohenSuth(HDC hdc, point p1, point p2, int xleft, int ytop, int xright, int ybottom,COLORREF c)
{
    point pStart;
    pStart.x = p1.x;
    pStart.y = p1.y;
    point pEnd;
    pEnd.x = p2.x;
    pEnd.y = p2.y;

    OutCode out1 = GetOutCode(pStart, xleft, ytop, xright, ybottom);
    OutCode out2 = GetOutCode(pEnd, xleft, ytop, xright, ybottom);
    cout <<"hello1" << endl;
    while ((out1.All || out2.All) && !(out1.All & out2.All))
    {
        int xi, yi;
        cout << "hello2"<<endl;
        if (out1.All)
        {
            if (out1.left)
                VIntersect(pStart, pEnd, xleft, &xi, &yi);
            else if (out1.top)
                HIntersect(pStart, pEnd, ytop, &xi, &yi);
            else if (out1.right)
                VIntersect(pStart, pEnd, xright, &xi, &yi);
            else
                HIntersect(pStart, pEnd, ybottom, &xi, &yi);
            pStart.x = xi;
            pStart.y = yi;

            out1 = GetOutCode(pStart, xleft, ytop, xright, ybottom);
            cout << "HERE1" << endl;
        }
        else
        {
            if (out2.left)
                VIntersect(pStart, pEnd, xleft, &xi, &yi);
            else if (out2.top)
                HIntersect(pStart, pEnd, ytop, &xi, &yi);
            else if (out2.right)
                VIntersect(pStart, pEnd, xright, &xi, &yi);
            else
                HIntersect(pStart, pEnd, ybottom, &xi, &yi);
            pEnd.x = xi;
            pEnd.y = yi;
            out2 = GetOutCode(pEnd, xleft, ytop, xright, ybottom);
            cout << "HERE2" << endl;
        }
    }
    if (!out1.All && !out2.All){
            //drawLine_directMethod(hdc, pStart.x,pStart.y,pEnd.x,pEnd.y, c);
            cout << "HERE3" <<endl;
    }
}

void DrawPolygon(HDC hdc, vector<point> p, COLORREF color)
{
    for (int i = 0; i < p.size() - 1; i++)
    {
        MidPointLine(hdc, p[i], p[i + 1], color);
    }
    MidPointLine(hdc, p[0], p[p.size() - 1], color);
}



void MidPointLine(HDC hdc, point p1, point p2, COLORREF c)
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    if ((dx >= 0 && 0 <= dy && dy <= dx) || (dx < 0 && 0 >= dy && dy >= dx)) // 0 < slope < 1
    {
        if (p1.x > p2.x)
        {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = dx - 2 * dy;
        int d1 = 2 * (dx - dy);
        int d2 = -2 * dy;
        SetPixel(hdc, x, y, c);
        while (x < p2.x)
        {
            if (d <= 0)
            {
                d += d1;
                y++;
            }
            else
            {
                d += d2;
            }
            x++;
            SetPixel(hdc, x, y, c);
        }
    }
    else if ((dx >= 0 && dy > dx) || (dx < 0 && dy < dx)) // slope > 1
    {
        if (p1.y > p2.y)
        {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = 2 * dx - 2 * dy;
        SetPixel(hdc, x, y, c);

        while (y < p2.y)
        {
            if (d <= 0)
            {
                d += d1;
            }
            else
            {
                d += d2;
                x++;
            }
            y++;

            SetPixel(hdc, x, y, c);
        }
    }
    else if ((dx >= 0 && dy < -dx) || (dx < 0 && dy > -dx)) // slope < -1
    {
        if (p1.y > p2.y)
        {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = 2 * dx + dy;
        int d1 = 2 * (dx + dy);
        int d2 = 2 * dx;
        SetPixel(hdc, x, y, c);

        while (y < p2.y)
        {
            if (d <= 0)
            {
                d += d1;
                x--;
            }
            else
            {
                d += d2;
            }
            y++;

            SetPixel(hdc, x, y, c);
        }
    }
    else
    {
        if (p1.x > p2.x)
        {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = -dx - 2 * dy;
        int d1 = -2 * dy;
        int d2 = 2 * (-dx - dy);
        SetPixel(hdc, x, y, c);
        while (x < p2.x)
        {
            if (d <= 0)
            {
                d += d1;
            }
            else
            {
                d += d2;
                y--;
            }
            x++;
            SetPixel(hdc, x, y, c);
        }
    }
}
