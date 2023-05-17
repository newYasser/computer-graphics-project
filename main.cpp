#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include<cmath>
#include <windows.h>
#include<stack>
using namespace std;

#define LINE_DDA 1
#define LINE_MID_POINT 2
#define LINE_PARAMETRIC 3
#define CIRCLE_DIRECT 4
#define CIRCLE_POLAR 5
#define CIRCLE_ITERATIVE_POLAR 6
#define CIRCLE_MIDPOINT 7
#define CIRCLE_MODIFIED_MIDPOINT 8
#define FILLING_CIRCLE_WITH_LINES 9
#define FILLING_CIRCLE_WITH_CIRCLES 10
#define FILLING_SQUARE_WITH_HERMIT_CURVE 11
#define FILLING_RECTANGLE_WITH_BEZIER_CURVE 12
#define FILLING_CONVEX 13
#define FILLING_NON_CONVEX 14
#define FLOOD_FILL_RECURSIVE 15
#define FLOOD_FILL_NON_RECURSIVE 16
#define CARDINAL_SPLINE_CURVE 17
#define ELLIPSE_DIRECT 18
#define ELLIPSE_POLAR 19
#define ELLIPSE_MIDPOINT 20
#define CLIPPING_USING_RECTANGLE_POINT 21
#define CLIPPING_USING_RECTANGLE_LINE 22
#define CLIPPING_USING_RECTANGLE_POLYGON 23
#define CLIPPING_USING_SQUARE_LINE 24
#define CLIPPING_USING_SQUARE_POLYGON 25
#define DRAW_RECTANGLE 26
#define DRAW_POLYGON 27
#define CIRCLE_AND_FILL 28
#define TWO_CIRCLES_AND_FILL 29
#define OTHER_OPTIONS_SAVE 30
#define OTHER_OPTIONS_RELOAD 31
#define OTHER_OPTIONS_CLEAR 32
#define COLOR_RED 33
#define COLOR_GREEN 34
#define COLOR_BLUE 35
#define COLOR_BLACK 36
#define COLOR_WHITE 37
#define COLOR_YELLOW 38
#define COLOR_ORANGE 39
#define COLOR_PURPLE 40
#define COLOR_PINK 41
#define COLOR_BROWN 42
#define COLOR_GRAY 43
#define COLOR_CYAN 44
#define filling_arr_size 400



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

typedef struct {
    int right, left;
} filling_arr[filling_arr_size];

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

int Round(double);


void drawLine_DDA(HDC , point , point, COLORREF);
void drawLine_parametric(HDC, point, point, COLORREF);
void MidPointLine(HDC, point, point, COLORREF);

void DrawEightPoints(HDC, int, int, int, int, COLORREF);
void drawCircle_Direct(HDC, point, point, COLORREF);
void drawCircle_Polar(HDC, point, point, COLORREF);
void drawCircle_IterativePolar(HDC, point, point, COLORREF);
void drawCircle_MidPoint(HDC, point, point, COLORREF);
void drawCircle_ModifiedMidPoint(HDC, point, point, COLORREF);

int getQuarter(point center,point radius,point p);
void drawCircle_FillingWithLines(HDC, point, point,int, COLORREF);
void drawCircle_FillingWithCircles(HDC, point, point, int, COLORREF);

void drawTwoCirclesAndFill(HDC, point, point,point,point , COLORREF);
void FillIntersectingArea(HDC , point , double , point , double , COLORREF );

void swap_point(point& one, point& two);
void initialize_array(filling_arr array);
void get_sky_line(point one, point two, filling_arr array);
void draw_edge(point point_arr[], int n, filling_arr array);
void draw_sky_line(filling_arr array, HDC hdc, COLORREF c);
void convex_fill(vector<point> p,  HDC hdc, COLORREF c);
void not_recursive_flood_fill(COLORREF boarder_color, COLORREF filling_color, HDC hdc, point p);
void Recursive_FloodFill(HDC hdc, point p, COLORREF currentColor, COLORREF filledColor);
void DrawBezierCurve(HDC hdc, point P0, point P1, point P2, point P3, COLORREF color);
void draw_rectangleWithBezierCurve(HDC , point ,point , COLORREF );
void draw_squareWithHermiteCurve(HDC , point ,point , COLORREF );

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

void DrawHermiteCurve(HDC hdc, point p1, point T1, point p2, point T2, COLORREF color);
void DrawCardinalSpline(HDC, vector<point> , int , double , COLORREF );
void Draw4points(HDC , int , int , int , int , COLORREF );
void DrawEllipseDirect(HDC , point p, int , int , COLORREF );
void DrawEllipsePolar(HDC , point , int , int , COLORREF );
void DrawEllipseMidpoint(HDC , point , int , int , COLORREF );
double CalcRadius(point, point );

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
    static int currColor=COLOR_RED;
    static point p1;
    static bool circleFillingFlag=false;
    static point p2;
    static point p3;


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
                    screen.emplace_back(DRAW_RECTANGLE, points, currColor);
                    //points.clear();
                    break;
                case DRAW_POLYGON:
                    DrawPolygon(hdc, points, c);
                    screen.emplace_back(DRAW_POLYGON, points, currColor);
                    //points.clear();
                    break;
                case CIRCLE_AND_FILL: {

                    if (points.size() == 0) {
                        cout << "To use this method you have to click 2 clicks\n"
                                "1- the first one is the center.\n"
                                "2- the second one is the radius.\n";


                    } else if (points.size() == 1) {
                        cout << "great you have entered the center" << endl;
                        cout << "now enter the radius" << endl;
                    } else {
                        p1 = points[points.size() - 2];
                        p2 = points[points.size() - 1];
                        drawCircle_ModifiedMidPoint(hdc, p1, p2, c);
                        int radius = (int) CalcRadius(p1, p2);
                        point l1s;
                        point l1e;
                        point l2s;
                        point l2e;
                        l1s.x = p1.x - radius;
                        l1s.y = p1.y;
                        l1e.x = p1.x + radius;
                        l1e.y = p1.y;
                        l2s.x = p1.x;
                        l2s.y = p1.y - radius;
                        l2e.x = p1.x;
                        l2e.y = p1.y + radius;
                        MidPointLine(hdc, l1s, l1e, c);
                        vector <point> ptemp1;
                        ptemp1.push_back(l1s);
                        ptemp1.push_back(l1e);

                        MidPointLine(hdc, l2s, l2e, c);
                        vector <point> ptemp2;
                        ptemp2.push_back(l2s);
                        ptemp2.push_back(l2e);
                        screen.emplace_back(CIRCLE_AND_FILL, points, currColor);
                        ptemp1.clear();
                        ptemp2.clear();
                        points.clear();
                        cout<<"Now to fill the circle you have to click on filling"<<endl;
                        circleFillingFlag=true;
                    }

                    break;
                }
                case LINE_MID_POINT:
                    if (points.size() == 0) {
                        cout << "Please Enter 2 points at least ( start point and end point )" << endl;
                        break;
                    } else if (points.size() == 1) {
                        cout << "great you have entered the start point" << endl;
                        cout << "now enter the end point" << endl;
                        break;
                    } else {
                        MidPointLine(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout << "The Shape is : Line and the Algorithm is : Mid Point" << endl;
                        cout << "Start Point : (" << points[points.size() - 2].x << "," << points[points.size() - 2].y
                             << ")" << endl;
                        cout << "End Point : (" << points[points.size() - 1].x << "," << points[points.size() - 1].y
                             << ")" << endl;
                        screen.emplace_back(LINE_MID_POINT, points, currColor);
                        points.clear();
                        break;
                    }
                case LINE_DDA:
                    if (points.size() == 0) {
                        cout << "Please Enter 2 points at least ( start point and end point )" << endl;
                        break;
                    } else if (points.size() == 1) {
                        cout << "great you have entered the start point" << endl;
                        cout << "now enter the end point" << endl;
                        break;
                    } else {
                        drawLine_DDA(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout << "The Shape is : Line and the Algorithm is : DDA" << endl;
                        cout << "Start Point : (" << points[points.size() - 2].x << "," << points[points.size() - 2].y
                             << ")" << endl;
                        cout << "End Point : (" << points[points.size() - 1].x << "," << points[points.size() - 1].y
                             << ")" << endl;
                        screen.emplace_back(LINE_DDA, points, currColor);
                        points.clear();
                        break;
                    }
                case LINE_PARAMETRIC:
                    if (points.size() == 0) {
                        cout << "Please Enter 2 points at least ( start point and end point )" << endl;
                        break;
                    } else if (points.size() == 1) {
                        cout << "great you have entered the start point" << endl;
                        cout << "now enter the end point" << endl;
                        break;
                    } else {
                        drawLine_parametric(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout << "The Shape is : Line and the Algorithm is : Parametric" << endl;
                        cout << "Start Point : (" << points[points.size() - 2].x << "," << points[points.size() - 2].y
                             << ")" << endl;
                        cout << "End Point : (" << points[points.size() - 1].x << "," << points[points.size() - 1].y
                             << ")" << endl;
                        screen.emplace_back(LINE_PARAMETRIC, points, currColor);
                        points.clear();
                        break;
                    }
                case CIRCLE_DIRECT:
                    if (points.size() == 0) {
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius"
                             << endl;
                        break;
                    } else if (points.size() == 1) {
                        cout << "great you have entered the center" << endl;
                        cout << "now enter the radius" << endl;
                        break;
                    } else {
                        drawCircle_Direct(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout << "The Shape is: Circle and the Algorithm is: Direct" << endl;
                        cout << " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y
                             << endl;
                        cout << " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y
                             << endl;
                        screen.emplace_back(CIRCLE_DIRECT, points, currColor);
                        points.clear();
                        break;
                    }
                case CIRCLE_POLAR:
                    if (points.size() == 0) {
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius"
                             << endl;
                        break;
                    } else if (points.size() == 1) {
                        cout << "great you have entered the center" << endl;
                        cout << "now enter the radius" << endl;
                        break;
                    } else {
                        drawCircle_Polar(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout << "The Shape is: Circle and the Algorithm is: Polar" << endl;
                        cout << " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y
                             << endl;
                        cout << " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y
                             << endl;
                        screen.emplace_back(CIRCLE_POLAR, points, currColor);
                        points.clear();
                        break;
                    }
                case CIRCLE_ITERATIVE_POLAR:
                    if (points.size() == 0) {
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius"
                             << endl;
                        break;
                    } else if (points.size() == 1) {
                        cout << "great you have entered the center" << endl;
                        cout << "now enter the radius" << endl;
                        break;
                    } else {
                        drawCircle_IterativePolar(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout << "The Shape is: Circle and the Algorithm is: Iterative Polar" << endl;
                        cout << " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y
                             << endl;
                        cout << " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y
                             << endl;
                        screen.emplace_back(CIRCLE_ITERATIVE_POLAR, points, currColor);
                        points.clear();
                        break;
                    }
                case CIRCLE_MIDPOINT:
                    if (points.size() == 0) {
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius"
                             << endl;
                        break;
                    } else if (points.size() == 1) {
                        cout << "great you have entered the center" << endl;
                        cout << "now enter the radius" << endl;
                        break;
                    } else {
                        drawCircle_MidPoint(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout << "The Shape is: Circle and the Algorithm is: Midpoint" << endl;
                        cout << " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y
                             << endl;
                        cout << " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y
                             << endl;
                        screen.emplace_back(CIRCLE_MIDPOINT, points, currColor);
                        points.clear();
                        break;
                    }
                case CIRCLE_MODIFIED_MIDPOINT:
                    if (points.size() == 0) {
                        cout << "Please Enter 2 points at least first one is the center and the second is the radius"
                             << endl;
                        break;
                    } else if (points.size() == 1) {
                        cout << "great you have entered the center" << endl;
                        cout << "now enter the radius" << endl;
                        break;
                    } else {
                        drawCircle_ModifiedMidPoint(hdc, points[points.size() - 2], points[points.size() - 1], c);
                        cout << "The Shape is: Circle and the Algorithm is: Modified Midpoint" << endl;
                        cout << " 1st point: " << points[points.size() - 2].x << " " << points[points.size() - 2].y
                             << endl;
                        cout << " 2nd point: " << points[points.size() - 1].x << " " << points[points.size() - 1].y
                             << endl;
                        screen.emplace_back(CIRCLE_MODIFIED_MIDPOINT, points, currColor);
                        points.clear();
                        break;
                    }

                case FILLING_CIRCLE_WITH_LINES:
                {
                    if(!circleFillingFlag){
                        cout << "please click first draw circle with fill" << endl;
                        points.clear();
                    }else{
                        if(points.size()<1){
                            cout<<"please enter quarter you want to fill"<<endl;
                        }
                        else{
                            p3 = points[points.size() - 1];
                            int quarter = getQuarter(p1, p2, p3);
                            drawCircle_FillingWithLines(hdc,p1,p2,quarter,c);
                            cout << "The Shape is: Circle and the Algorithm is: Filling Circle With Lines" << endl;
                            cout << " 1st point (Center of the circle): " << p1.x << " " << p1.y << endl;
                            cout << " 2nd point (point on the circle to get radius):" << p2.x << " " << p2.y << endl;
                            cout << " 3rd point (point to get the number of quarter to fill with lines): " << p3.x << " "<< p3.y << endl;
                            screen.emplace_back(FILLING_CIRCLE_WITH_LINES, points, currColor);
                            p1.x=p1.y=p2.x=p2.y=-1;
                            circleFillingFlag = false;
                            points.clear();
                        }
                    }


                    break;
                }
                case FILLING_CIRCLE_WITH_CIRCLES:
                    if(!circleFillingFlag){
                        cout << "please click first draw circle with fill" << endl;
                        points.clear();
                    }else{
                        if(points.size()<1){
                            cout<<"please enter quarter you want to fill with circles"<<endl;
                        }
                        else{
                            p3 = points[points.size() - 1];
                            int quarter = getQuarter(p1, p2, p3);
                            drawCircle_FillingWithCircles(hdc,p1,p2,quarter,c);
                            cout << "The Shape is: Circle and the Algorithm is: Filling Circle With Circles" << endl;
                            cout << " 1st point (Center of the circle): " << p1.x << " " << p1.y << endl;
                            cout << " 2nd point (point on the circle to get radius):" << p2.x << " " << p2.y << endl;
                            cout << " 3rd point (point to get the number of quarter to fill with circles): " << p3.x << " "<< p3.y << endl;
                            screen.emplace_back(FILLING_CIRCLE_WITH_CIRCLES, points, currColor);
                            circleFillingFlag = false;
                            points.clear();
                        }
                    }
                    break;
                case TWO_CIRCLES_AND_FILL:
                    if(points.size() ==0 ){
                        cout << "To use this method you have to click 4 clicks\n"
                                "1- the first one is the center of the first circle.\n"
                                "2- the second one is the radius of the first circle.\n"
                                "3- the third one is the center of the second circle.\n"
                                "4- the fourth one is the radius of the second circle.\n";
                    }else if(points.size() == 1){
                        cout<<"great you have entered the center of the first circle"<<endl;
                        cout<<"now enter the radius of the first circle and the center and radius of the second circle"<<endl;
                    } else if(points.size() == 2){
                        cout<<"great you have entered the center and radius of the first circle"<<endl;
                        cout<<"now enter the center and radius of the second circle"<<endl;
                    }
                    else if (points.size()==3){
                        cout<<"great you have entered the center and radius of the first circle and the center of the second circle"<<endl;
                        cout<<"now enter the radius of the second circle"<<endl;
                    }
                    else{
                        drawTwoCirclesAndFill(hdc, points[points.size() - 4], points[points.size() - 3], points[points.size() - 2], points[points.size() - 1], c);
                        cout<< "The Shape is: Two Circles and the Algorithm is: Two Circles and Fill" << endl;
                        cout<< " 1st point (Center of the first circle): " << points[points.size() - 4].x << " " << points[points.size() - 4].y << endl;
                        cout<< " 2nd point (point on the first circle to get radius):" << points[points.size() - 3].x << " " << points[points.size() - 3].y << endl;
                        cout<< " 3rd point (Center of the second circle): " << points[points.size() - 2].x << " " << points[points.size() - 2].y << endl;
                        cout<< " 4th point (point on the second circle to get radius):" << points[points.size() - 1].x << " " << points[points.size() - 1].y << endl;
                        screen.emplace_back(TWO_CIRCLES_AND_FILL, points,currColor);
                        points.clear();
                    }
                    break;
                case FILLING_SQUARE_WITH_HERMIT_CURVE:
                    if(points.size() ==0 ) {
                        cout << "To use this method you have to click 2 clicks\n"
                                "1- the first one is the top left point of the square.\n"
                                "2- the second one is the bottom right point of the square.\n";
                    }
                    else if (points.size()==1){
                        cout<<"great you have entered the top left point of the square"<<endl;
                        cout<<"now enter the bottom right point of the square"<<endl;
                    }else {
                        draw_squareWithHermiteCurve(hdc, points[points.size() - 1],points[points.size()- 2],c);
                        cout << "The Shape is: Square and the Algorithm is: Filling Square With Hermit Curve" << endl;
                        cout << " 1st point (top left point of the square): " << points[points.size() - 1].x << " " << points[points.size() - 1].y << endl;
                        cout << " 2nd point (bottom right point of the square):" << points[points.size() - 2].x << " " << points[points.size() - 2].y << endl;
                        screen.emplace_back(FILLING_SQUARE_WITH_HERMIT_CURVE, points, currColor);
                        points.clear();
                    }
                    break;
                case FILLING_RECTANGLE_WITH_BEZIER_CURVE:
                    if(points.size() ==0 ) {
                        cout << "To use this method you have to click 2 clicks\n"
                                "1- the first one is the top left point of the rectangle.\n"
                                "2- the second one is the bottom right point of the rectangle.\n";
                    }
                    else if (points.size()==1){
                        cout<<"great you have entered the top left point of the rectangle"<<endl;
                        cout<<"now enter the bottom right point of the rectangle"<<endl;
                    }else {
                        draw_rectangleWithBezierCurve(hdc, points[points.size() - 1],points[points.size()- 2],c);
                        cout << "The Shape is: Rectangle and the Algorithm is: Filling Rectangle With Bezier Curve" << endl;
                        cout << " 1st point (top left point of the rectangle): " << points[points.size() - 1].x << " " << points[points.size() - 1].y << endl;
                        cout << " 2nd point (bottom right point of the rectangle):" << points[points.size() - 2].x << " " << points[points.size() - 2].y << endl;
                        screen.emplace_back(FILLING_RECTANGLE_WITH_BEZIER_CURVE, points, currColor);
                        points.clear();
                    }
                    break;
                case FILLING_CONVEX:
                    if(points.size() ==0 ) {
                       cout<<"To use this method you have to click 4 clicks\n"
                               "1- the first one is the first point of the convex.\n"
                               "2- the second one is the second point of the convex.\n"
                               "3- the third one is the third point of the convex.\n"
                               "4- the fourth one is the fourth point of the convex.\n";

                    }else if(points.size() == 1){
                        cout<<"great you have entered the first point of the convex"<<endl;
                        cout<<"now enter the second and third and forth points of the convex"<<endl;
                    } else if(points.size() == 2){
                        cout<<"great you have entered the first and second points of the convex"<<endl;
                        cout<<"now enter the third and fourth point of the convex"<<endl;
                    } else if(points.size() == 3){
                        cout<<"great you have entered the first, second and third points of the convex"<<endl;
                        cout<<"now enter the fourth point of the convex"<<endl;
                    }else {
                        convex_fill(points, hdc, c);
                        cout << "The Shape is: Convex and the Algorithm is: Filling Convex" << endl;
                        cout << " 1st point (first point of the convex): " << points[points.size() - 4].x << " "
                             << points[points.size() - 4].y << endl;
                        cout << " 2nd point (second point of the convex):" << points[points.size() - 3].x << " "
                             << points[points.size() - 3].y << endl;
                        cout << " 3rd point (third point of the convex): " << points[points.size() - 2].x << " "
                             << points[points.size() - 2].y << endl;
                        cout << " 4th point (fourth point of the convex):" << points[points.size() - 1].x << " "
                                << points[points.size() - 1].y << endl;
                        screen.emplace_back(FILLING_CONVEX, points, currColor);
                        points.clear();
                    }
                    break;
                case FILLING_NON_CONVEX:

                case FLOOD_FILL_RECURSIVE:
                    break;
                case FLOOD_FILL_NON_RECURSIVE:
                    break;
                case ELLIPSE_DIRECT:{
                    if (points.empty()) {
                        cout << "To use this method you have to click 3 clicks\n"
                                "1- the first one is the center.\n"
                                "2- the second one is the horizontal radius.\n"
                                "3- and the third one is the vertical radius.\n";
                    }else if(points.size() < 3){
                        cout << "Please Enter the 3 points you only clicked " << points.size() << "points" << endl;
                        if(points.size()  == 1){
                            cout << "You still didn't click for the horizontal radius (a) and the vertical radius (b)." << endl;
                        }
                        else if(points.size() == 2){
                            cout << "You still didn't click for the the vertical radius (b)" <<endl;
                        }
                    }else {

                        int a = CalcRadius(points[0], points[1]);
                        int b = CalcRadius(points[0], points[2]);
                        DrawEllipseDirect(hdc, points[0], a, b, c);
                        screen.emplace_back(ELLIPSE_DIRECT, points, currColor);
                        points.clear();
                    }
                    break;
                }
                case ELLIPSE_POLAR: {
                    if (points.empty()) {
                        cout << "To use this method you have to click 3 clicks\n"
                                "1- the first one is the center.\n"
                                "2- the second one is the horizontal radius.\n"
                                "3- and the third one is the vertical radius.\n";
                    }else if(points.size() < 3){
                        cout << "Please Enter the 3 points you only clicked " << points.size() << "points" << endl;
                        if(points.size()  == 1){
                            cout << "You still didn't click for the horizontal radius (a) and the vertical radius (b)." << endl;
                        }
                        else if(points.size() == 2){
                            cout << "You still didn't click for the the vertical radius (b)" <<endl;
                        }
                    }else {
                        int a = CalcRadius(points[0], points[1]);
                        int b = CalcRadius(points[0], points[2]);
                        cout << a << endl;
                        cout << b << endl;
                        DrawEllipsePolar(hdc, points[0], a, b, c);
                        screen.emplace_back(ELLIPSE_POLAR, points, currColor);
                        points.clear();
                    }
                    break;
                }
                case ELLIPSE_MIDPOINT:
                {
                    if (points.empty()) {
                        cout << "To use this method you have to click 3 clicks\n"
                                "1- the first one is the center.\n"
                                "2- the second one is the horizontal radius.\n"
                                "3- and the third one is the vertical radius.\n";
                    }else if(points.size() < 3){
                        cout << "Please Enter the 3 points you only clicked " << points.size() << "points" << endl;
                        if(points.size()  == 1){
                            cout << "You still didn't click for the horizontal radius (a) and the vertical radius (b)." << endl;
                        }
                        else if(points.size() == 2){
                            cout << "You still didn't click for the the vertical radius (b)" <<endl;
                        }
                    }else {
                        int a = CalcRadius(points[0], points[1]);
                        int b = CalcRadius(points[0], points[2]);
                        cout << a << endl;
                        cout << b << endl;
                        DrawEllipseMidpoint(hdc, points[0], a, b, c);
                        screen.emplace_back(ELLIPSE_MIDPOINT, points, currColor);
                        points.clear();
                    }
                    break;
                }
                case CLIPPING_USING_RECTANGLE_LINE:
                    if (points.size() == 0) {
                        cout << "Please Enter the line 2 points and draw a Rectangle" << endl;
                        break;
                    } else if (points.size() == 2) {
                        cout << "Please Enter the line 2 points and draw a a rec" << endl;
                        DrawRectangle(hdc,points[0],points[1],c);
                        screen.emplace_back(DRAW_RECTANGLE,points,currColor);
                        break;
                    }else if(points.size() < 4){
                        cout << "Enter 2 points to draw a line." << endl;
                    }
                    else if (points.size() >= 4) {
                        CohenSuth(hdc, points[points.size() - 2], points[points.size() - 1],
                                  points[points.size() - 4].x, points[points.size() - 3].y, points[points.size() - 3].x,
                                  points[points.size() - 4].y, c);
                        screen.emplace_back(CLIPPING_USING_RECTANGLE_LINE, points,currColor);
                        points.clear();
                    } else {
                        cout << "Something went wrong please try again" << endl;
                        break;
                    }
                    break;
                case CLIPPING_USING_RECTANGLE_POLYGON:
                    PolygonClip(hdc, points, points.size(), points[0].x, points[1].y, points[1].x, points[0].y);
                    screen.emplace_back(CLIPPING_USING_RECTANGLE_POLYGON, points,currColor);
                    points.clear();
                    break;
                case CARDINAL_SPLINE_CURVE:
                    if(points.size() < 3){
                        cout << "You have to Enter points more than 3" << endl;
                    }
                    DrawCardinalSpline( hdc, points, points.size(),1,c);
                    screen.emplace_back(CARDINAL_SPLINE_CURVE,points,currColor);
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
                                SendMessage(hWnd,WM_COMMAND, c, 0);
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
                    currColor = COLOR_ORANGE;
                    break;
                case COLOR_RED:
                    c = RGB(255, 0, 0);
                    currColor = COLOR_RED;
                    break;
                case COLOR_GREEN:
                    c = RGB(0, 255, 0);
                    currColor = COLOR_GREEN;
                    break;
                case COLOR_BLUE:
                    c = RGB(0, 0, 255);
                    currColor = COLOR_BLUE;
                    break;
                case COLOR_BLACK:
                    c = RGB(0, 0, 0);
                    currColor = COLOR_BLACK;
                    break;
                case COLOR_WHITE:
                    c = RGB(255, 255, 255);
                    currColor = COLOR_WHITE;
                    break;
                case COLOR_YELLOW:
                    c = RGB(255, 255, 0);
                    currColor = COLOR_YELLOW;
                    break;
                case COLOR_PURPLE:
                    c = RGB(128, 0, 128);
                    currColor = COLOR_PURPLE;
                    break;
                case COLOR_BROWN:
                    c = RGB(165, 42, 42);
                    currColor = COLOR_BROWN;
                    break;
                case COLOR_PINK:
                    c = RGB(255, 192, 203);
                    currColor = COLOR_PINK;
                    break;
                case COLOR_GRAY:
                    c = RGB(128, 128, 128);
                    currColor = COLOR_GRAY;
                    break;
                case COLOR_CYAN:
                    c = RGB(0, 255, 255);
                    currColor = COLOR_CYAN;
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
    AppendMenu(hDraw, MF_STRING, TWO_CIRCLES_AND_FILL, "TWO CIRCLES AND FILL");
    AppendMenu(hDraw, MF_STRING, CIRCLE_AND_FILL, "Circle and fill");


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
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hColor, "Color");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hOtherOptions, "Other");


    SetMenu(hWnd, hMenu);
}

// starting point of execution



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

void drawLine_parametric(HDC hdc, point p1, point p2, COLORREF c) {
    double x, y;
    for (double t = 0; t < 1; t += 0.0001)
    {
        x = p1.x + t * (p2.x - p1.x);
        y = p1.y + t * (p2.y - p1.y);
        SetPixel(hdc, round(x), round(y), c);
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
        x = round(radius * sin(theta));
        y = round(radius * cos(theta));
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
    int radius =  sqrt(pow(y2 - y1, 2)+ pow(x2 - x1, 2));
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

int getQuarter(point center,point radius,point p){
    if(p.x<center.x && p.y<center.y) return 2;
    if(p.x>center.x && p.y<center.y) return 1;
    if(p.x>center.x && p.y>center.y) return 4;
    if(p.x<center.x && p.y>center.y) return 3;
    return 0;
}
void drawCircle_FillingWithLines(HDC hdc, point  p1, point p2,int quarter, COLORREF color){
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    int radius = sqrt(pow(y2 - y1, 2)+ pow(x2 - x1, 2));
    // i want to draw lines between the center and the radius point in specific quarter
    // and use MidPoint line method to draw the lines
    double sAngel = 0;
    double eAngel = 0;
    switch (quarter){
        case 1:
            sAngel = 0;
            eAngel = 90;
            break;
        case 2:
            sAngel = 90;
            eAngel = 180;
            break;
        case 3:
            sAngel = 180;
            eAngel = 270;
            break;
        case 4:
            sAngel = 270;
            eAngel = 360;
            break;
    }
    for (double i = sAngel; i < eAngel; i+=0.1) {
        double xe = x1 + static_cast<int>(radius * cos(i * 3.14159 / 180.0));
        double ye = y1 - static_cast<int>(radius * sin(i * 3.14159 / 180.0));
        point pE ;
        pE.x = xe;
        pE.y = ye;
        MidPointLine(hdc,p1,pE,color);
    }
}

void drawCircle_FillingWithCircles(HDC hdc, point  p1, point p2,int quarter, COLORREF color) {
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    double radius = CalcRadius(p1, p2);
    double sAngel = 0;
    double eAngel = 0;
    switch (quarter) {
        case 1:
            sAngel = 0;
            eAngel = 90;
            break;
        case 2:
            sAngel = 90;
            eAngel = 180;
            break;
        case 3:
            sAngel = 180;
            eAngel = 270;
            break;
        case 4:
            sAngel = 270;
            eAngel = 360;
            break;
    }
    int sRadius = 0;
    int eRadius = radius;
    for(; sRadius<2*eRadius; sRadius+=1)
    {
        for (double i = sAngel; i < eAngel; i += 0.1)
        {
            int x = x1 + static_cast<int>(sRadius / 2 * cos(i * 3.14159 / 180.0));
            int y = y1 - static_cast<int>(sRadius / 2 * sin(i * 3.14159 / 180.0));
            SetPixel(hdc, x, y, color);
        }

    }
}



void drawTwoCirclesAndFill(HDC hdc, point p1 , point p2,point p3 ,point p4, COLORREF color){
    drawCircle_ModifiedMidPoint(hdc,p1,p2,color);
    drawCircle_ModifiedMidPoint(hdc,p3,p4,color);
    double radius1=CalcRadius(p1,p2);
    double radius2=CalcRadius(p3,p4);
    double distance=CalcRadius(p1,p3);
    if(abs(distance - (radius1 + radius2)) < 1e-6)
        cout<<"Tangent Circles\n";
    else if(distance > (radius1 + radius2))
        cout<<"Disjoint Circles\n";
    else if(p1.x==p2.x && p1.y==p2.y && radius2!=radius1)
        cout<<"Concentric Circles\n";
    else if(distance < abs(radius1 - radius2) && (radius1+radius2)>distance)
        cout<<"Nested Circles\n";
    else{
        cout<<"Intersecting Circles\n";
        FillIntersectingArea(hdc,p1,radius1,p3,radius2,color);
    }
}

void FillIntersectingArea(HDC hdc, point circle1Center, double circle1Radius, point circle2Center, double circle2Radius, COLORREF color) {
    int left = max(circle1Center.x - circle1Radius, circle2Center.x - circle2Radius);
    int top = max(circle1Center.y - circle1Radius, circle2Center.y - circle2Radius);
    int right = min(circle1Center.x + circle1Radius, circle2Center.x + circle2Radius);
    int bottom = min(circle1Center.y + circle1Radius, circle2Center.y + circle2Radius);
    for (int y = top; y <= bottom; y++) {
        for (int x = left; x <= right; x++) {
            double distance1 = sqrt(pow(x - circle1Center.x, 2) + pow(y - circle1Center.y, 2));
            double distance2 = sqrt(pow(x - circle2Center.x, 2) + pow(y - circle2Center.y, 2));
            if (distance1 <= circle1Radius && distance2 <= circle2Radius) {
                SetPixel(hdc, x, y, color);
            }
        }
    }
}

void swap_point(point& one, point& two)
{
    point temp = one;
    one = two;
    two = temp;
}

void initialize_array(filling_arr array)
{
    for (int i = 0; i < filling_arr_size; i++)
    {
        array[i].left = MAXINT;
        array[i].right = MININT;
    }
}



void get_sky_line(point one, point two, filling_arr array)
{
    if (one.y == two.y)return;
    if (one.y > two.y)swap_point(one, two);
    double dx = two.x - one.x, dy = two.y - one.y, x = one.x;
    int y = one.y;
    double m = dx / dy;
    while (y < two.y) {
        if (x < array[y].left)array[y].left = (int)ceil(x);
        if (x > array[y].right)array[y].right = (int)floor(x);
        x += m;
        y++;
    }
}


void draw_edge(vector<point> point_arr,  filling_arr array)
{
    point temp1 = point_arr[point_arr.size() -1];
    for (int i = 0; i < point_arr.size(); i++)
    {
        point temp2 = point_arr[i];
        get_sky_line(temp1, temp2, array);
        temp1 = point_arr[i];
    }
}

void draw_sky_line(filling_arr array, HDC hdc, COLORREF c) {
    for (int i = 0; i < filling_arr_size; i++)
    {
        if (array[i].left < array[i].right)
        {
            for (int x = array[i].left; x <= array[i].right; x++)
            {
                SetPixel(hdc, x, i, c);
            }
        }
    }
}


void convex_fill(vector<point> p ,  HDC hdc, COLORREF c) {
    filling_arr array;
    initialize_array(array);
    draw_edge(p,  array);
    draw_sky_line(array, hdc, c);
}


void not_recursive_flood_fill(COLORREF boarder_color, COLORREF filling_color, HDC hdc, point p) {
    std::stack<point> st;
    st.push(p);
    while (!st.empty()) {
        point vertex = st.top();
        st.pop();
        COLORREF c = GetPixel(hdc,vertex.x, vertex.y);
        if (c == filling_color || c == boarder_color)continue;
        SetPixel(hdc, vertex.x, vertex.y, filling_color);
        st.push(point(vertex.x + 1, vertex.y));
        st.push(point(vertex.x -1, vertex.y));
        st.push(point(vertex.x , vertex.y+1));
        st.push(point(vertex.x , vertex.y-1));
    }
}
void Recursive_FloodFill(HDC hdc, point p, COLORREF currentColor, COLORREF filledColor)
{
    COLORREF c = GetPixel(hdc, p.x, p.y);
    if (c != currentColor)
        return;
    SetPixel(hdc, p.x, p.y, filledColor);
    Recursive_FloodFill(hdc, {p.x + 1, p.y}, currentColor, filledColor);
    Recursive_FloodFill(hdc, {p.x, p.y + 1}, currentColor, filledColor);
    Recursive_FloodFill(hdc, {p.x - 1, p.y}, currentColor, filledColor);
    Recursive_FloodFill(hdc, {p.x, p.y - 1}, currentColor, filledColor);
}
void draw_rectangleWithBezierCurve(HDC hdc, point topLeft,point bottomRight, COLORREF boarder_color){
    DrawRectangle(hdc, topLeft, bottomRight, boarder_color);
    point p1;
    p1.x = min(topLeft.x, bottomRight.x);
    p1.y = min(topLeft.y, bottomRight.y);
    point p2;
    p2.x = max(topLeft.x, bottomRight.x);
    p2.y = min(topLeft.y, bottomRight.y);
    point p3;
    p3.x = max(topLeft.x, bottomRight.x);
    p3.y = max(topLeft.y, bottomRight.y);
    point p4;
    p4.x = min(topLeft.x, bottomRight.x);
    p4.y = max(topLeft.y, bottomRight.y);
    for (int i = p1.y; i <= p4.y; i++)
    {
        DrawBezierCurve(hdc, p4, {p4.x + 10, p4.y - 10}, {p3.x - 10, p3.y - 10}, p3, boarder_color);
        DrawBezierCurve(hdc, p1, {p1.x + 10, p1.y + 10}, {p2.x - 10, p2.y + 10}, p2, boarder_color);
        p1.y += 1;
        p2.y += 1;
        p3.y -= 1;
        p4.y -= 1;
    }

}

void DrawBezierCurve(HDC hdc, point P0, point P1, point P2, point P3, COLORREF color)
{
    point T0;
    T0.x = (3 * (P1.x - P0.x));
    T0.y = 3 * (P1.y - P0.y);
    point T1;
    T1.x = (3 * (P3.x - P2.x));
    T1.y = 3 * (P3.y - P2.y);
    DrawHermiteCurve(hdc, P0, T0, P3, T1, color);
}


void draw_squareWithHermiteCurve(HDC hdc, point topLeft,point bottomRight, COLORREF boarder_color) {
    int width = bottomRight.x - topLeft.x;
    bottomRight= { topLeft.x + width, topLeft.y + width };
    point topRight = { bottomRight.x, topLeft.y };
    point bottomLeft = { topLeft.x, bottomRight.y };


    MidPointLine(hdc, topLeft, topRight, boarder_color);
    MidPointLine(hdc, topRight, bottomRight, boarder_color);
    MidPointLine(hdc, bottomRight, bottomLeft, boarder_color);
    MidPointLine(hdc, bottomLeft, topLeft, boarder_color);

    point p1;
    p1.x = min(topLeft.x, bottomRight.x);
    p1.y = min(topLeft.y, bottomRight.y);
    point p2;
    p2.x = max(topLeft.x, bottomRight.x);
    p2.y = min(topLeft.y, bottomRight.y);
    point p3;
    p3.x = max(topLeft.x, bottomRight.x);
    p3.y = max(topLeft.y, bottomRight.y);
    point p4;
    p4.x = min(topLeft.x, bottomRight.x);
    p4.y = max(topLeft.y, bottomRight.y);
    for (int i = p1.x; i <= p2.x; i++)
    {
        DrawHermiteCurve(hdc, p1, {50, 50}, p4, {-50, 50}, boarder_color);
        DrawHermiteCurve(hdc, p2, {-50, 50}, p3, {50, 50}, boarder_color);
        p1.x += 1;
        p2.x -= 1;
        p3.x -= 1;
        p4.x += 1;
    }


}
void DrawHermiteCurve(HDC hdc, point p1, point T1, point p2, point T2, COLORREF color)
{

    double alpha0 = p1.x,
            alpha1 = T1.x,
            alpha2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,
            alpha3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
    double beta0 = p1.y,
            beta1 = T1.y,
            beta2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,
            beta3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t * t,
                t3 = t2 * t;
        double x = alpha0 + alpha1 * t + alpha2 * t2 + alpha3 * t3;
        double y = beta0 + beta1 * t + beta2 * t2 + beta3 * t3;

        SetPixel(hdc, round(x), round(y), color);
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
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        int xi, yi;
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
        }
    }
    if (!out1.All && !out2.All) {
        drawLine_DDA(hdc, pStart, pEnd, c);
    }
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
    for (int i = 0; i < n; i++)vlist.push_back(point(p[i].x, p[i].y));
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



void DrawCardinalSpline(HDC hdc, vector<point> p, int n, double c, COLORREF color)
{
    vector<point> t(n);
    for (int i = 1; i < n - 1; i++)
    {
        t[i].x = (c / 2) * (p[i + 1].x - p[i - 1].x);
        t[i].y = (c / 2) * (p[i + 1].y - p[i - 1].y);
    }
    t[0].x = (c / 2) * (p[1].x - p[0].x);
    t[0].y = (c / 2) * (p[1].y - p[0].y);

    t[n - 1].x = (c / 2) * (p[n - 1].x - p[n - 2].x);
    t[n - 1].y = (c / 2) * (p[n - 1].y - p[n - 2].y);
    for (int i = 0; i < n - 1; i++)
    {
        DrawHermiteCurve(hdc, p[i], t[i], p[i + 1], t[i + 1], color);
    }
}

void Draw4points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc - x, yc - y, color);
}
double CalcRadius(point pc,point p)
{
    return sqrt(pow((p.x - pc.x),2) + pow((p.y - pc.y),2));
}
void DrawEllipseDirect(HDC hdc, point p, int a, int b, COLORREF c)
{
    int xc = p.x;
    int yc = p.y;
    int x = 0;
    double y = b;
    Draw4points(hdc, xc, yc, x, round(y), c);
    while (x * b * b < y * a * a)
    {
        x++;
        y = b * sqrt(1 - (x * x * 1.0) / (a * a));
        Draw4points(hdc, xc, yc, x, round(y), c);
    }
    int y1 = 0;
    double x1 = a;
    Draw4points(hdc, xc, yc, round(x1), y1, c);

    while (x1 * b * b > y1 * a * a)
    {
        y1++;
        x1 = a * sqrt(1 - (y1 * y1 * 1.0) / (b * b));
        Draw4points(hdc, xc, yc, round(x1), y1, c);

    }
}
void DrawEllipsePolar(HDC hdc, point p, int a, int b, COLORREF c)
{
    int xc = p.x;
    int yc = p.y;
    double x = a;
    double y = 0;
    double theta = 0;
    double dtheta = 1.0 / ((a + b));
    double cd = cos(dtheta);
    double sd = sin(dtheta);

    Draw4points(hdc, xc, yc, round(x), round(y), c);
    while (x > 0)
    {
        x = a * cos(theta);
        y = b * sin(theta);
        theta += dtheta;
        Draw4points(hdc, xc, yc, round(x), round(y), c);
    }
}

void DrawEllipseMidpoint(HDC hdc, point p, int a, int b, COLORREF c)

{
    int xc = p.x;
    int yc = p.y;
    int x = 0, y = b;
    int b2 = b * b;
    int a2 = a * a;
    double d = b2 + a2 * pow((b - 0.5), 2) - a2 * b2;
    Draw4points(hdc, xc, yc, x, y, c);
    while (x * b * b < y * a * a)
    {
        if (d <= 0)
        {
            d += b2 * (2 * x + 3);
            x++;
        }
        else
        {
            d += b2 * (2 * x + 3) + a2 * (-2 * y + 2);
            x++;
            y--;
        }
        Draw4points(hdc, xc, yc, x, y, c);
    }
    x = a;
    y = 0;
    d = b2 * pow((a - 0.5), 2) + a2 - a2 * b2;
    Draw4points(hdc, xc, yc, x, y, c);
    while (x * b * b > y * a * a)
    {
        if (d <= 0)
        {
            d += a2 * (2 * y + 3);
            y++;
        }
        else
        {
            d += a2 * (2 * y + 3) + b2 * (-2 * x + 2);
            x--;
            y++;
        }
        Draw4points(hdc, xc, yc, x, y, c);
    }
}