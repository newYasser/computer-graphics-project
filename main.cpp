#include <iostream>
#include <vector>
#include <list>
#include <fstream>
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
#define DRAW_RECTANGLE 27
#define DRAW_POLYGON 28
#define OTHER_OPTIONS_SAVE 29
#define OTHER_OPTIONS_RELOAD 30
#define OTHER_OPTIONS_CLEAR 31

struct point
{
    int x, y;
    point(){
        x = -1;
        y = -1;
    }
    point(int x,int y){
        this->x = x;
        this->y = y;
    }
};

struct algorithm{
    int id;
    vector<point>points;
    algorithm(){
        id = -1;
    }
    algorithm(int id,vector<point>&points){
        this->points = points;
        this->id = id;
    }
};
union OutCode
{
    unsigned All : 4;
    struct
    {
        unsigned left : 1, top : 1, right : 1, bottom : 1;
    };
};

typedef vector<point> VertexList;
typedef bool (*IsInFunc)(point& v,int edge);
typedef point (*IntersectFunc)(point& v1,point& v2,int edge);
HMENU hMenu;
LRESULT CALLBACK WindowProcedure(HWND,UINT,WPARAM,LPARAM);
void AddMenus(HWND);
int round(double);
void drawLine_directMethod(HDC ,point,point, COLORREF);
void DrawRectangle(HDC hdc,point, point,COLORREF);
OutCode GetOutCode(point , int , int , int , int );
void VIntersect(point, point, int , int*, int*);
void HIntersect(point , point , int, int*, int*);
void CohenSuth(HDC , point , point , int , int , int , int , COLORREF );
void DrawPolygon(HDC, vector<point> , COLORREF);
void MidPointLine(HDC, point, point , COLORREF );
VertexList ClipWithEdge(VertexList p,int edge,IsInFunc In,IntersectFunc Intersect);
bool InLeft(point& v,int edge);
bool InRight(point& v,int edge);
bool InTop(point& v,int edge);
bool InBottom(point& v,int edge);
point VIntersect(point& ,point&,int );
point HIntersect(point& ,point& ,int );
point VIntersect(point& ,point& ,int );
point HIntersect(point& ,point& ,int );
void PolygonClip(HDC hdc,vector<point>& ,int ,int ,int ,int ,int );
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
    HDC hdc = GetDC(hWnd) ;
    static COLORREF c = RGB(255,0,0);
    static  vector<point>points;
    static list<algorithm>screen;

    switch(msg)
   {
       case WM_LBUTTONDOWN: {
           point p;
           p.x = LOWORD(lp);
           p.y = HIWORD(lp);
           points.push_back(p);
           break;
       }
       /* when something is clicked anything here will be performed
        * the w parameter determine which item is clicked
        */

       case WM_COMMAND: {
           switch (wp) {
               case DRAW_RECTANGLE:
                   if(points.size() < 2){
                       cout << "Please Enter 2 points at least" << endl;
                       break;
                   }
                   DrawRectangle(hdc, points[points.size() - 2], points[points.size() - 1], c);
                   screen.emplace_back(DRAW_RECTANGLE,points);
                   points.clear();
                   break;
               case DRAW_POLYGON:
                   DrawPolygon(hdc, points, c);
                   screen.emplace_back(DRAW_POLYGON,points);
                   points.clear();
                   break;
               case LINE_MID_POINT:
                   MidPointLine(hdc,points[points.size() - 2], points[points.size() - 1],c);
                   screen.emplace_back(LINE_MID_POINT,points);
                   points.clear();
                   break;
               case CLIPPING_USING_RECTANGLE_LINE:
                   if(points.size() == 0){
                       cout << "Please Enter the line 2 points and draw a Rectangle" <<endl;
                       break;
                   }
                   else if(points.size() == 2){
                       cout << "Please Enter the line 2 points and draw a line" <<endl;
                       break;
                   }
                   else if (points.size() >= 4) {
                       CohenSuth(hdc, points[points.size() - 2], points[points.size() - 1],
                                 points[points.size() - 4].x, points[points.size() - 3].y, points[points.size() - 3].x,
                                 points[points.size() - 4].y, c);
                       screen.emplace_back(CLIPPING_USING_RECTANGLE_LINE,points);
                       points.clear();
                   }
                   else{
                       cout << "Something went wrong please try again" <<endl;
                       break;
                   }
                   break;
               case CLIPPING_USING_RECTANGLE_POLYGON:
                   PolygonClip(hdc,points,points.size(),points[0].x,points[1].y,points[1].x,points[0].y);
                   screen.emplace_back(CLIPPING_USING_RECTANGLE_POLYGON,points);
                   points.clear();
                   break;
               case OTHER_OPTIONS_RELOAD:
               {
                   ifstream saveFile("save.txt");
                   string line;
                   vector<int>nums;
                   string num;
                   while(getline(saveFile,line)){
                       for(char c : line){
                           if(isdigit(c)){
                               num += c;
                           }else if(c == ' '){
                              nums.push_back(stoi(num));
                           }
                           if(c == ' '){
                               num.clear();
                           }
                       }



                   }

                   break;
               }
               case OTHER_OPTIONS_SAVE:
                   ofstream saveFile("save.txt");
                   for (auto algo : screen) {
                       saveFile << algo.id;
                        for(auto point :algo.points){
                            saveFile << point.x << ' ' << point.y << ' ';
                        }
                        saveFile << '\n';
                   }
                   saveFile.close();

                   break;
           }
           break;
       }
       case WM_CREATE:
           AddMenus(hWnd);
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
    HMENU hDraw = CreateMenu();
    HMENU hOtherOptions = CreateMenu();


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

    AppendMenu(hDraw,MF_STRING,DRAW_RECTANGLE,"RECTANGLE");
    AppendMenu(hDraw,MF_STRING,DRAW_POLYGON,"POLYGON");

    AppendMenu(hOtherOptions,MF_STRING,OTHER_OPTIONS_SAVE,"Save");
    AppendMenu(hOtherOptions,MF_STRING,OTHER_OPTIONS_RELOAD,"Reload");
    AppendMenu(hOtherOptions,MF_STRING,OTHER_OPTIONS_CLEAR,"Clear");


    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hLineMenu,"Line");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hCircleMenu,"Circle");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFillingMenu,"Filling");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hCurveMenu,"Curve");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hEllipseMenu,"Ellipse");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hClippingMenu,"Clipping");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hDraw,"Draw");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hOtherOptions,"Other");

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

void DrawRectangle(HDC hdc,point p1,point p2,COLORREF c){
    point p3,p4;
    p3.x = p1.x;
    p3.y = p2.y;
    p4.x = p2.x;
    p4.y = p1.y;
    MidPointLine(hdc,p1,p4,c);
    MidPointLine(hdc,p1,p3,c);
    MidPointLine(hdc,p2,p3,c);
    MidPointLine(hdc,p2,p4,c);
}



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
            drawLine_directMethod(hdc, pStart,pEnd, c);
            cout << "HERE3" <<endl;
    }
    cout << "HERE4" << endl;
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
VertexList ClipWithEdge(VertexList p,int edge,IsInFunc In,IntersectFunc Intersect)
{
    VertexList OutList;
    point v1=p[p.size()-1];
    bool v1_in=In(v1,edge);
    for(int i=0;i<(int)p.size();i++)
    {
        point v2=p[i];
        bool v2_in=In(v2,edge);
        if(!v1_in && v2_in)
        {
            OutList.push_back(Intersect(v1,v2,edge));
            OutList.push_back(v2);
        }else if(v1_in && v2_in) OutList.push_back(v2);
        else if(v1_in) OutList.push_back(Intersect(v1,v2,edge));
        v1=v2;
        v1_in=v2_in;
    }
    return OutList;
}
bool InLeft(point& v,int edge)
{
    return v.x>=edge;
}
bool InRight(point& v,int edge)
{
    return v.x<=edge;
}
bool InTop(point& v,int edge)
{
    return v.y>=edge;
}
bool InBottom(point& v,int edge)
{
    return v.y<=edge;
}
point VIntersect(point& v1,point& v2,int xedge)
{
    point res;
    res.x=xedge;
    res.y=v1.y+(xedge-v1.x)*(v2.y-v1.y)/(v2.x-v1.x);
    return res;
}
point HIntersect(point& v1,point& v2,int yedge)
{
    point res;
    res.y=yedge;
    res.x=v1.x+(yedge-v1.y)*(v2.x-v1.x)/(v2.y-v1.y);
    return res;
}
void PolygonClip(HDC hdc,vector<point> &p,int n,int xleft,int ytop,int xright,int ybottom)
{
    VertexList vlist;
    for(int i=2;i<n;i++)vlist.push_back(point(p[i].x,p[i].y));
    vlist=ClipWithEdge(vlist,xleft,InLeft,VIntersect);
    vlist=ClipWithEdge(vlist,ytop,InTop,HIntersect);
    vlist=ClipWithEdge(vlist,xright,InRight,VIntersect);
    vlist=ClipWithEdge(vlist,ybottom,InBottom,HIntersect);
    point v1=vlist[vlist.size()-1];
    for(int i=0;i<(int)vlist.size();i++)
    {
        point v2=vlist[i];
        MoveToEx(hdc,round(v1.x),round(v1.y),NULL);
        LineTo(hdc,round(v2.x),round(v2.y));
        v1=v2;
    }
}