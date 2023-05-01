#include <iostream>
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







HMENU hMenu;
LRESULT CALLBACK WindowProcedure(HWND,UINT,WPARAM,LPARAM);
void AddMenus(HWND);
void AddControls(HWND);




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
   switch(msg)
   {
       /* when something is clicked anything here will be performed
        * the w parameter determine which item is clicked
        * */
       case WM_COMMAND:

           switch (wp)
           {
               case 1:
                   MessageBeep(MB_OK);
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



