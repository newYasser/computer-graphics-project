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
#define COLOR_RED 32
#define COLOR_GREEN 33
#define COLOR_BLUE 34
#define COLOR_BLACK 35
#define COLOR_WHITE 36
#define COLOR_YELLOW 37
#define COLOR_ORANGE 38
#define COLOR_PURPLE 39
#define COLOR_PINK 40
#define COLOR_BROWN 41
#define COLOR_GRAY 42
#define COLOR_CYAN 43



struct point {
    int x, y;

    point() {
        x = -1;
        y = -1;
    }

    point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

struct algorithm {
    int id;
    vector <point> points;
    int color;

    algorithm() {
        id = -1;
        color = COLOR_BLACK;
    }

    algorithm(int id, vector <point> &points, int color) {
        this->points = points;
        this->id = id;
        this->color = color;
    }
};

union OutCode {
    unsigned All: 4;
    struct {
        unsigned left: 1, top: 1, right: 1, bottom: 1;
    };
};

typedef vector <point> VertexList;

typedef bool (*IsInFunc)(point &v, int edge);

typedef point (*IntersectFunc)(point &v1, point &v2, int edge);

HMENU hMenu;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);

int round(double);

template<typename T>
T sqrt(T);

template<typename T>
T pow(T, unsigned int);

void drawLine_DDA(HDC , point , point, COLORREF);

void drawLine_parametric(HDC, point, point, COLORREF);

void MidPointLine(HDC, point, point, COLORREF);

void DrawEightPoints(HDC, int, int, int, int, COLORREF);

void drawCircle_Direct(HDC, point, point, COLORREF);

void drawCircle_Polar(HDC, point, point, COLORREF);

void drawCircle_IterativePolar(HDC, point, point, COLORREF);

void drawCircle_MidPoint(HDC, point, point, COLORREF);

void drawCircle_ModifiedMidPoint(HDC, point, point, COLORREF);


void DrawRectangle(HDC hdc, point, point, COLORREF);

OutCode GetOutCode(point, int, int, int, int);

void VIntersect(point, point, int, int *, int *);

void HIntersect(point, point, int, int *, int *);

void CohenSuth(HDC, point, point, int, int, int, int, COLORREF);

void DrawPolygon(HDC, vector <point>, COLORREF);

VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect);

bool InLeft(point &v, int edge);

bool InRight(point &v, int edge);

bool InTop(point &v, int edge);

bool InBottom(point &v, int edge);

point VIntersect(point &, point &, int);

point HIntersect(point &, point &, int);

void PolygonClip(HDC hdc, vector <point> &, int, int, int, int, int);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hInstance = hInst;
    wc.hInstance = hInst;
    wc.lpszClassName = (L"WindowClass");
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc)) {
        cout << "Register Failed" << endl;
        return -1;
    }

    CreateWindowW((L"WindowClass"), (L"Graphics Project"),
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, nullptr, NULL, NULL, NULL);

    MSG msg = {nullptr};
    while (GetMessage(&msg, nullptr, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    HDC hdc = GetDC(hWnd);
    static COLORREF c = RGB(255, 0, 0);
    static vector <point> points;
    static list <algorithm> screen;

    switch (msg) {
        case WM_LBUTTONDOWN: {
            point p;
            p.x = LOWORD(lp);
            p.y = HIWORD(lp);
            cout<<"you clicked at "<<p.x<<" "<<p.y<<endl;
            points.push_back(p);
            break;
        }
/* when something is clicked anything here will be performed
 * the w parameter determine which item is clicked
 */

        case WM_COMMAND: {

            switch (wp) {
                case DRAW_RECTANGLE:
                    if (points.size() < 2) {
                        cout << "Please Enter 2 points at least" << endl;
                        break;
                    }
                    DrawRectangle(hdc, points[points.size() - 2], points[points.size() - 1], c);
                    screen.emplace_back(DRAW_RECTANGLE, points, c);
                    points.clear();
                    break;
                case DRAW_POLYGON:
                    DrawPolygon(hdc, points, c);
                    screen.emplace_back(DRAW_POLYGON, points, c);
                    points.clear();
                    break;
                case LINE_MID_POINT:
                    if (points.size() < 2) {
                        cout << "Please Enter 2 points at least ( start point and end point )" << endl;
                        points.clear();
                        break;
                    }
                    else{
                        MidPointLine(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is : Line and the Algorithm is : Mid Point" << endl;
                        cout<<"Start Point : ("<<points[points.size() - 2].x<<","<<points[points.size() - 2].y<<")"<<endl;
                        cout<<"End Point : ("<<points[points.size() - 1].x<<","<<points[points.size() - 1].y<<")"<<endl;
                        screen.emplace_back(LINE_MID_POINT, points,c);
                        points.clear();
                        break;
                    }
                case LINE_DDA:
                    if (points.size() < 2) {
                        cout << "Please Enter 2 points at least ( start point and end point )" << endl;
                        points.clear();
                        break;
                    }
                    else{
                        drawLine_DDA(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is : Line and the Algorithm is : DDA" << endl;
                        cout<<"Start Point : ("<<points[points.size() - 2].x<<","<<points[points.size() - 2].y<<")"<<endl;
                        cout<<"End Point : ("<<points[points.size() - 1].x<<","<<points[points.size() - 1].y<<")"<<endl;
                        screen.emplace_back(LINE_DDA, points,c);
                        points.clear();
                        break;
                    }
                case LINE_PARAMETRIC:
                    if(points.size() < 2){
                        cout << "Please Enter 2 points at least ( start point and end point )" << endl;
                        points.clear();
                        break;
                    }
                    else{
                        drawLine_parametric(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is : Line and the Algorithm is : Parametric" << endl;
                        cout<<"Start Point : ("<<points[points.size() - 2].x<<","<<points[points.size() - 2].y<<")"<<endl;
                        cout<<"End Point : ("<<points[points.size() - 1].x<<","<<points[points.size() - 1].y<<")"<<endl;
                        screen.emplace_back(LINE_PARAMETRIC, points,c);
                        points.clear();
                        break;
                    }
                case CIRCLE_DIRECT:
                    if(points.size() < 2){
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius" << endl;
                        points.clear();
                        break;
                    }
                    else{
                        drawCircle_Direct(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is: Circle and the Algorithm is: Direct" << endl;
                        cout<< " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y << endl;
                        cout<< " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y << endl;
                        screen.emplace_back(CIRCLE_DIRECT, points,c);
                        points.clear();
                        break;
                    }
                case CIRCLE_POLAR:
                    if(points.size() < 2){
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius" << endl;
                        points.clear();
                        break;
                    }
                    else{
                        drawCircle_Polar(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is: Circle and the Algorithm is: Polar" << endl;
                        cout<< " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y << endl;
                        cout<< " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y << endl;
                        screen.emplace_back(CIRCLE_POLAR, points,c);
                        points.clear();
                        break;
                    }
                case CIRCLE_ITERATIVE_POLAR:
                    if(points.size() < 2){
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius" << endl;
                        points.clear();
                        break;
                    }
                    else{
                        drawCircle_IterativePolar(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is: Circle and the Algorithm is: Iterative Polar" << endl;
                        cout<< " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y << endl;
                        cout<< " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y << endl;
                        screen.emplace_back(CIRCLE_ITERATIVE_POLAR, points,c);
                        points.clear();
                        break;
                    }
                case CIRCLE_MIDPOINT:
                    if(points.size() < 2){
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius" << endl;
                        points.clear();
                        break;
                    }
                    else{
                        drawCircle_MidPoint(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is: Circle and the Algorithm is: Midpoint" << endl;
                        cout<< " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y << endl;
                        cout<< " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y << endl;
                        screen.emplace_back(CIRCLE_MIDPOINT, points,c);
                        points.clear();
                        break;
                    }
                case CIRCLE_MODIFIED_MIDPOINT:
                    if(points.size() < 2){
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius" << endl;
                        points.clear();
                        break;
                    }
                    else{
                        drawCircle_ModifiedMidPoint(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is: Circle and the Algorithm is: Modified Midpoint" << endl;
                        cout<< " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y << endl;
                        cout<< " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y << endl;
                        screen.emplace_back(CIRCLE_MODIFIED_MIDPOINT, points,c);
                        points.clear();
                        break;
                    }
                case CLIPPING_USING_RECTANGLE_LINE:
                    if (points.size() == 0) {
                        cout << "Please Enter the line 2 points and draw a Rectangle" << endl;
                        break;
                    } else if (points.size() == 2) {
                        cout << "Please Enter the line 2 points and draw a line" << endl;
                        break;
                    } else if (points.size() >= 4) {
                        CohenSuth(hdc, points[points.size() - 2], points[points.size() - 1],
                                  points[points.size() - 4].x, points[points.size() - 3].y, points[points.size() - 3].x,
                                  points[points.size() - 4].y, c);
                        screen.emplace_back(CLIPPING_USING_RECTANGLE_LINE, points,c);
                        points.clear();
                    } else {
                        cout << "Something went wrong please try again" << endl;
                        break;
                    }
                    break;
                case CLIPPING_USING_RECTANGLE_POLYGON:
                    PolygonClip(hdc, points, points.size(), points[0].x, points[1].y, points[1].x, points[0].y);
                    screen.emplace_back(CLIPPING_USING_RECTANGLE_POLYGON, points,c);
                    points.clear();
                    break;
                case OTHER_OPTIONS_CLEAR:
                    screen.clear();
                    points.clear();
                    InvalidateRect(hWnd, nullptr, TRUE);
                    cout<< "Screen Cleared" << endl;
                    break;
                case OTHER_OPTIONS_RELOAD: {
                    screen.clear();
                    points.clear();

                    ifstream saveFile("save.txt");
                    string line;
                    while (getline(saveFile, line)) {
                        bool colorFlag = false;
                        int id ;
                        bool idFlag=true;
                        string num;
                        bool pointFlag=false;
                        point p;
                        for(int i=0;i<line.size();i++){
                            if(line[i]==' ' && idFlag){
                                id=stoi(num);
                                idFlag=false;
                                colorFlag=true;
                                num="";
                            }else if (colorFlag && line[i]==' '){
                                c=stoi(num);
                                SendMessage(hWnd,WM_COMMAND, id, 0);
                                colorFlag=false;
                                num="";
                            }
                            else if(line[i]==' '&& !idFlag){
                                if(pointFlag ){
                                    p.y=stoi(num);
                                    pointFlag=false;
                                    SendMessage(hWnd,WM_LBUTTONDOWN,0,MAKELPARAM(p.x,p.y));

                                }
                                else{
                                    p.x=stoi(num);
                                    pointFlag=true;
                                }
                                num="";
                            }
                            else{
                                num+=line[i];
                            }

                        }
                        // SEND MESSAGE TO DRAW
                        SendMessage(hWnd, WM_COMMAND, id, 0);
                    }
                    break;
                }
                case COLOR_ORANGE:
                    c = RGB(255, 165, 0);
                    break;
                case COLOR_RED:
                    c = RGB(255, 0, 0);
                    break;
                case COLOR_GREEN:
                    c = RGB(0, 255, 0);
                    break;
                case COLOR_BLUE:
                    c = RGB(0, 0, 255);
                    break;
                case COLOR_BLACK:
                    c = RGB(0, 0, 0);
                    break;
                case COLOR_WHITE:
                    c = RGB(255, 255, 255);
                    break;
                case COLOR_YELLOW:
                    c = RGB(255, 255, 0);
                    break;
                case COLOR_PURPLE:
                    c = RGB(128, 0, 128);
                    break;
                case COLOR_BROWN:
                    c = RGB(165, 42, 42);
                    break;
                case COLOR_PINK:
                    c = RGB(255, 192, 203);
                    break;
                case COLOR_GRAY:
                    c = RGB(128, 128, 128);
                    break;
                case COLOR_CYAN:
                    c = RGB(0, 255, 255);
                    break;
                case OTHER_OPTIONS_SAVE:
                    ofstream saveFile("save.txt");
                    for (auto algo: screen) {
                        saveFile << algo.id<<' ';
                        saveFile << algo.color << ' ';
                        for (auto point: algo.points) {
                            saveFile << point.x << ' ' << point.y << ' ';
                        }

                        saveFile <<'\n';
                    }
                    saveFile.close();
                    cout << "Saved Successfully" << endl;
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
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &ps.rcPaint, hBrush);
            EndPaint(hWnd, &ps);
        }
            break;
        default:
            return DefWindowProc(hWnd, msg, wp, lp);

    }
}

void AddMenus(HWND hWnd) {
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
    HMENU hColor=CreateMenu();


    AppendMenu(hLineMenu, MF_STRING, LINE_DDA, "DDA");
    AppendMenu(hLineMenu, MF_STRING, LINE_MID_POINT, "Mid Point");
    AppendMenu(hLineMenu, MF_STRING, LINE_PARAMETRIC, "Parametric");

    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_DIRECT, "Direct");
    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_POLAR, "Polar");
    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_ITERATIVE_POLAR, "Iterative Polar");
    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_MIDPOINT, "Mid Point");
    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_MODIFIED_MIDPOINT, "Modified Midpoint");

    AppendMenu(hFillingMenu, MF_STRING, FILLING_CIRCLE_WITH_LINES, "Circle with lines");
    AppendMenu(hFillingMenu, MF_STRING, FILLING_CIRCLE_WITH_CIRCLES, "Circle with other circles");
    AppendMenu(hFillingMenu, MF_STRING, FILLING_SQUARE_WITH_HERMIT_CURVE, "Square with Hermit");
    AppendMenu(hFillingMenu, MF_STRING, FILLING_RECTANGLE_WITH_BEZIER_CURVE, "Rectangle With Bezier");
    AppendMenu(hFillingMenu, MF_STRING, FILLING_CONVEX, "Convex");
    AppendMenu(hFillingMenu, MF_STRING, FILLING_NON_CONVEX, "Non Convex");
    AppendMenu(hFloodFillMenu, MF_STRING, FLOOD_FILL_RECURSIVE, "Recursive");
    AppendMenu(hFloodFillMenu, MF_STRING, FLOOD_FILL_NON_RECURSIVE, "Non Recursive");

    AppendMenu(hFillingMenu, MF_POPUP, (UINT_PTR) hFloodFillMenu, "Flood Fill");

    AppendMenu(hCurveMenu, MF_STRING, CARDINAL_SPLINE_CURVE, "Cardinal Spline");


    AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_DIRECT, "Direct");
    AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_POLAR, "Polar");
    AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_MIDPOINT, "Mid Point");

    AppendMenu(hClippingRectangleMenu, MF_STRING, CLIPPING_USING_RECTANGLE_POINT, "Point");
    AppendMenu(hClippingRectangleMenu, MF_STRING, CLIPPING_USING_RECTANGLE_LINE, "Line");
    AppendMenu(hClippingRectangleMenu, MF_STRING, CLIPPING_USING_RECTANGLE_POLYGON, "Polygon");
    AppendMenu(hClippingSquareMenu, MF_STRING, CLIPPING_USING_SQUARE_LINE, "Line");
    AppendMenu(hClippingSquareMenu, MF_STRING, CLIPPING_USING_SQUARE_POLYGON, "Polygon");


    AppendMenu(hClippingMenu, MF_POPUP, (UINT_PTR) hClippingSquareMenu, "Square");
    AppendMenu(hClippingMenu, MF_POPUP, (UINT_PTR) hClippingRectangleMenu, "Rectangle");

    AppendMenu(hDraw, MF_STRING, DRAW_RECTANGLE, "RECTANGLE");
    AppendMenu(hDraw, MF_STRING, DRAW_POLYGON, "POLYGON");

    AppendMenu(hOtherOptions, MF_STRING, OTHER_OPTIONS_SAVE, "Save");
    AppendMenu(hOtherOptions, MF_STRING, OTHER_OPTIONS_RELOAD, "Reload");
    AppendMenu(hOtherOptions, MF_STRING, OTHER_OPTIONS_CLEAR, "Clear");

    AppendMenu(hColor, MF_STRING, COLOR_RED, "Red");
    AppendMenu(hColor, MF_STRING, COLOR_GREEN, "Green");
    AppendMenu(hColor, MF_STRING, COLOR_BLUE, "Blue");
    AppendMenu(hColor, MF_STRING, COLOR_BLACK, "Black");
    AppendMenu(hColor, MF_STRING, COLOR_WHITE, "White");
    AppendMenu(hColor, MF_STRING, COLOR_YELLOW, "Yellow");
    AppendMenu(hColor, MF_STRING, COLOR_PURPLE, "Purple");
    AppendMenu(hColor, MF_STRING, COLOR_ORANGE, "Orange");
    AppendMenu(hColor, MF_STRING, COLOR_GRAY, "Gray");
    AppendMenu(hColor, MF_STRING, COLOR_BROWN, "Brown");
    AppendMenu(hColor, MF_STRING, COLOR_PINK, "Pink");
    AppendMenu(hColor, MF_STRING, COLOR_CYAN, "Cyan");





    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hLineMenu, "Line");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hCircleMenu, "Circle");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFillingMenu, "Filling");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hCurveMenu, "Curve");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hEllipseMenu, "Ellipse");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hClippingMenu, "Clipping");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hDraw, "Draw");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hOtherOptions, "Other");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hColor, "Color");


    SetMenu(hWnd, hMenu);
}

// starting point of execution
int round(double point) {
    // check the nearest integer point ( to determine whether to round down or up )
    if (point - (int) point > 0.5)
        return (int) point + 1;
    else
        return (int) point;
}

int max(int x, int y) {
    return (x > y ? x : y);
}

template<typename T>
T pow(T x, unsigned int y) {
    if (y == 0)
        return 1;
    else if (y % 2 == 0)
        return pow(x, y / 2) * pow(x, y / 2);
    else
        return x * pow(x, y / 2) * pow(x, y / 2);
}

template<typename T>
T sqrt(T x) {
    T i = 0;
    while ((i * i) <= x) {
        i++;
    }
    return i - 1;
}

template<typename T>
T sin(T x) {
    // sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
    T sinx = x;
    for (int i = 1; i <= 5; i++) {
        if (i % 2 == 0) {
            sinx += (pow(x, 2 * i + 1) / (2 * i + 1));
        } else {
            sinx -= (pow(x, 2 * i + 1) / (2 * i + 1));
        }
    }
    return sinx;
}

template<typename T>
T cos(T x) {
    // cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + ...
    T cosx = 1;
    for (int i = 1; i <= 5; i++) {
        if (i % 2 == 0) {
            cosx += (pow(x, 2 * i) / (2 * i));
        } else {
            cosx -= (pow(x, 2 * i) / (2 * i));
        }
    }
    return cosx;
}

void MidPointLine(HDC hdc, point p1, point p2, COLORREF c) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    if ((dx >= 0 && 0 <= dy && dy <= dx) || (dx < 0 && 0 >= dy && dy >= dx)) // 0 < slope < 1
    {
        if (p1.x > p2.x) {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = dx - 2 * dy;
        int d1 = 2 * (dx - dy);
        int d2 = -2 * dy;
        SetPixel(hdc, x, y, c);
        while (x < p2.x) {
            if (d <= 0) {
                d += d1;
                y++;
            } else {
                d += d2;
            }
            x++;
            SetPixel(hdc, x, y, c);
        }
    } else if ((dx >= 0 && dy > dx) || (dx < 0 && dy < dx)) // slope > 1
    {
        if (p1.y > p2.y) {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = 2 * dx - 2 * dy;
        SetPixel(hdc, x, y, c);

        while (y < p2.y) {
            if (d <= 0) {
                d += d1;
            } else {
                d += d2;
                x++;
            }
            y++;

            SetPixel(hdc, x, y, c);
        }
    } else if ((dx >= 0 && dy < -dx) || (dx < 0 && dy > -dx)) // slope < -1
    {
        if (p1.y > p2.y) {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = 2 * dx + dy;
        int d1 = 2 * (dx + dy);
        int d2 = 2 * dx;
        SetPixel(hdc, x, y, c);

        while (y < p2.y) {
            if (d <= 0) {
                d += d1;
                x--;
            } else {
                d += d2;
            }
            y++;

            SetPixel(hdc, x, y, c);
        }
    } else {
        if (p1.x > p2.x) {
            swap(p1, p2);
            dx = p2.x - p1.x;
            dy = p2.y - p1.y;
        }
        int x = p1.x, y = p1.y;
        int d = -dx - 2 * dy;
        int d1 = -2 * dy;
        int d2 = 2 * (-dx - dy);
        SetPixel(hdc, x, y, c);
        while (x < p2.x) {
            if (d <= 0) {
                d += d1;
            } else {
                d += d2;
                y--;
            }
            x++;
            SetPixel(hdc, x, y, c);
        }
    }
}

void drawLine_DDA(HDC hdc, point p1, point p2, COLORREF color){
    // get x1,y1 and x2,y2
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    // calculate dx, dy
    int dx = x2 - x1, dy = y2 - y1;
    // calculate xinc, yinc
    double xinc = dx > 0 ? 1 : -1;
    double yinc;
    if (abs(dy) > abs(dx)) {
        yinc = dy > 0 ? 1 : -1;
    } else {
        yinc = (double) dy / dx * xinc;
    }
    if (abs(dx) <= abs(dy))
        xinc = (double) dx / dy * yinc;

    SetPixel(hdc,x1,y1,color);
    double x=x1,y=y1;
    // loop until x1=x2 and y1=y2
    if(abs(dx)>=abs(dy))
    {
        while(x!=x2)
        {
            x+=xinc;y+=yinc;
            SetPixel(hdc,round(x),round(y),color);
        }
    }
    else
    {
        while(y!=y2)
        {
            x+=xinc;y+=yinc;
            SetPixel(hdc,round(x),round(y),color);
        }
    }

}

void drawLine_parametric(HDC hdc, point p1, point p2, COLORREF color) {
    // get x1,y1 and x2,y2
    int x1 = p1.x; int y1 = p1.y;
    int x2 = p2.x; int y2 = p2.y;
    // calculate dx, dy
    int dx = x2 - x1;
    int dy = y2 - y1;
    // calculate slope
    float slope = (float) dy /(float) dx;
    // calculate x,y start
    int x = x1;
    auto y =(float) y1;
    // loop until x1=x2 and y1=y2
    while (x <= p2.x) {
        SetPixel(hdc, x, round(y), color);
        x += 1;
        y += slope;
    }
}

void DrawEightPoints(HDC hdc, int xCenter, int yCenter, int a, int b, COLORREF color) {
    SetPixel(hdc, xCenter + a, yCenter + b, color);
    SetPixel(hdc, xCenter + a, yCenter - b, color);
    SetPixel(hdc, xCenter - a, yCenter + b, color);
    SetPixel(hdc, xCenter - a, yCenter - b, color);
    SetPixel(hdc, xCenter + b, yCenter + a, color);
    SetPixel(hdc, xCenter + b, yCenter - a, color);
    SetPixel(hdc, xCenter - b, yCenter + a, color);
    SetPixel(hdc, xCenter - b, yCenter - a, color);

}

void drawCircle_Direct(HDC hdc, point p1, point p2, COLORREF color) {
    // get x1, y1, x2, y2 from p1, p2
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    // calculate radius of circle
    int r = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    // Initialize x and y to the starting point on the circle (x,y) = (0,radius)
    int x = 0, y = r;
    while (x <= y) {
        DrawEightPoints(hdc, x1, y1, x, y, color);
        // Increment x by 1
        x++;
        // Calculate y using the circle equation y = sqrt(r^2 - x^2)
        y = round(sqrt((double) r * r - x * x));
    }
}

void drawCircle_Polar(HDC hdc, point p1, point p2, COLORREF color) {
    // get x1, y1, x2, y2 from p1, p2
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    // calculate radius of circle
    int radius = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    // Initialize x and y to the starting point on the circle (x,y) = (0,radius)
    int x = 0, y = radius;
    // Initialize theta to 0
    double theta = 0;
    // Calculate the change in theta for each iteration of the loop
    double dtheta = 1.0 / radius;
    while (x <= y) {
        DrawEightPoints(hdc, x1, y1, x, y, color);
        theta += dtheta;
        x = round(radius * cos(theta));
        y = round(radius * sin(theta));
    }
}

void drawCircle_IterativePolar(HDC hdc, point p1, point p2, COLORREF color) {
    // get x1, y1, x2, y2 from p1, p2
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    // calculate radius of circle
    int radius = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    // Initialize x and y to the starting point on the circle (x, y) = (r, 0)
    double x = 0, y = radius;
    // Calculate the change in theta for each iteration of the loop
    double dtheta = 1.0 / radius;
    while (x <= y) {
        DrawEightPoints(hdc, x1, y1, round(x), round(y), color);
        double temp = x * cos(dtheta) - y * sin(dtheta);
        y = x * sin(dtheta) + y * cos(dtheta);
        x = temp;
    }

}

void drawCircle_MidPoint(HDC hdc, point p1, point p2, COLORREF color) {
    // get x1, y1, x2, y2 from p1, p2
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    // calculate radius of circle
    int radius = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    // initialize starting point
    int x = 0, y = radius;
    // calculate initial value of decision parameter
    int dParameter = 1 - radius;
    while (x <= y) {
        // draw 8 points
        DrawEightPoints(hdc, x1, y1, x, y, color);
        // update decision parameter
        if (dParameter < 0) {
            dParameter += 2 * x + 3;
        } else {
            dParameter += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void drawCircle_ModifiedMidPoint(HDC hdc, point p1, point p2, COLORREF color) {
    // get x1, y1, x2, y2 from p1, p2
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    // calculate radius of circle
    int radius =  pow(y2 - y1, 2)+ pow(x2 - x1, 2);
    // initialize starting point
    int x = 0, y = radius;
    // initialize decision parameter
    int dParmaeter = 1 - radius;
    // initialize constants
    int constant1 = 3;
    int constant2 = -2 * radius + 5;
    while (x <= y) {
        // draw eight points
        DrawEightPoints(hdc, x1, y1, x, y, color);
        // update decision parameter
        if (dParmaeter < 0) {
            dParmaeter += constant1;
            constant2 += 2;
        } else {
            dParmaeter += constant2;
            constant2 += 4;
            y--;
        }
        constant1 += 2;
        x++;
    }
}

void DrawRectangle(HDC hdc, point p1, point p2, COLORREF c) {
    point p3, p4;
    p3.x = p1.x;
    p3.y = p2.y;
    p4.x = p2.x;
    p4.y = p1.y;
    MidPointLine(hdc, p1, p4, c);
    MidPointLine(hdc, p1, p3, c);
    MidPointLine(hdc, p2, p3, c);
    MidPointLine(hdc, p2, p4, c);
}

OutCode GetOutCode(point p1, int xleft, int ytop, int xright, int ybottom) {
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

void VIntersect(point p1, point p2, int x, int *xi, int *yi) {
    *xi = x;
    *yi = p1.y + (x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
}

void HIntersect(point p1, point p2, int y, int *xi, int *yi) {
    *yi = y;
    *xi = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
}

void CohenSuth(HDC hdc, point p1, point p2, int xleft, int ytop, int xright, int ybottom, COLORREF c) {
    point pStart;
    pStart.x = p1.x;
    pStart.y = p1.y;
    point pEnd;
    pEnd.x = p2.x;
    pEnd.y = p2.y;

    OutCode out1 = GetOutCode(pStart, xleft, ytop, xright, ybottom);
    OutCode out2 = GetOutCode(pEnd, xleft, ytop, xright, ybottom);
    cout << "hello1" << endl;
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        int xi, yi;
        cout << "hello2" << endl;
        if (out1.All) {
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
        } else {
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
    if (!out1.All && !out2.All) {
        drawLine_DDA(hdc, pStart, pEnd, c);
        cout << "HERE3" << endl;
    }
    cout << "HERE4" << endl;
}

void DrawPolygon(HDC hdc, vector <point> p, COLORREF color) {
    for (int i = 0; i < p.size() - 1; i++) {
        MidPointLine(hdc, p[i], p[i + 1], color);
    }
    MidPointLine(hdc, p[0], p[p.size() - 1], color);
}

VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect) {
    VertexList OutList;
    point v1 = p[p.size() - 1];
    bool v1_in = In(v1, edge);
    for (int i = 0; i < (int) p.size(); i++) {
        point v2 = p[i];
        bool v2_in = In(v2, edge);
        if (!v1_in && v2_in) {
            OutList.push_back(Intersect(v1, v2, edge));
            OutList.push_back(v2);
        } else if (v1_in && v2_in) OutList.push_back(v2);
        else if (v1_in) OutList.push_back(Intersect(v1, v2, edge));
        v1 = v2;
        v1_in = v2_in;
    }
    return OutList;
}

bool InLeft(point &v, int edge) {
    return v.x >= edge;
}

bool InRight(point &v, int edge) {
    return v.x <= edge;
}

bool InTop(point &v, int edge) {
    return v.y >= edge;
}

bool InBottom(point &v, int edge) {
    return v.y <= edge;
}

point VIntersect(point &v1, point &v2, int xedge) {
    point res;
    res.x = xedge;
    res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
    return res;
}

point HIntersect(point &v1, point &v2, int yedge) {
    point res;
    res.y = yedge;
    res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
    return res;
}

void PolygonClip(HDC hdc, vector <point> &p, int n, int xleft, int ytop, int xright, int ybottom) {
    VertexList vlist;
    for (int i = 2; i < n; i++)vlist.push_back(point(p[i].x, p[i].y));
    vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
    vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
    vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
    vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);
    point v1 = vlist[vlist.size() - 1];
    for (int i = 0; i < (int) vlist.size(); i++) {
        point v2 = vlist[i];
        MoveToEx(hdc, round(v1.x), round(v1.y), NULL);
        LineTo(hdc, round(v2.x), round(v2.y));
        v1 = v2;
    }
}