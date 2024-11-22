#include "D3/3DGraphics.h"
#include <graphx.h>
#include <math.h>
#define PI 3.14159265358979323846

void intToStr(int N, char *str) {
    int i = 0;
    // Save the copy of the number for sign
    int sign = N;
    // If the number is negative, make it positive
    if (N < 0)
        N = -N;
    // Extract digits from the number and add them to the
    // string
    while (N > 0) {     
        // Convert integer digit to character and store
      	// it in the str
        str[i++] = N % 10 + '0';
      	N /= 10;
    } 
    // If the number was negative, add a minus sign to the
    // string
    if (sign < 0) {
        str[i++] = '-';
    }
    // Null-terminate the string
    str[i] = '\0';
    // Reverse the string to get the correct order
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

void D3G_Init() 
{
    gfx_Begin();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    DrawUI();
}
void DrawUI() 
{
    //Draw Square:
    //          x               y                           x              y
    gfx_Line(border,        border,                     320-border,    border);                 //UL-UR
    gfx_Line(border,        border,                     border,        240-border-ExtraBorder); //UL-DL
    gfx_Line(320-border,    border,                     320-border,    240-border-ExtraBorder); //UR-DR
    gfx_Line(border,        240 -border -ExtraBorder,   320-border,    240-border-ExtraBorder); //DL-DR

    Vector3 TOP = {0,100,0};
    Vector3 DOWN = {0,-100,0};
    Vector3 RIGHT = {100,0,0};
    Vector3 LEFT = {-100,0,0};
    Vector3 FORWARD = {0,0,100};
    Vector3 BACKWART = {0,0,-100};
    D3G_DrawLine(TOP, DOWN);
    D3G_DrawLine(RIGHT, LEFT);
    D3G_DrawLine(FORWARD, BACKWART);
}
#if Debug
void D3G_DrawDebugPoint(Vector3 pos) 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_red);
#pragma GCC diagnostic pop
    Vector2 Point = project(pos);
    //gfx_SetPixel(Point.x, Point.y);
    gfx_Circle(Point.x, Point.y,3);
    char  Debug1[16];
    char  Debug2[16];
    intToStr(Point.x, Debug1);
    intToStr(Point.y, Debug2);
    gfx_PrintStringXY("test2", 200, 100);
    gfx_PrintStringXY(Debug1, 100, 200);
    gfx_PrintStringXY(Debug2, 200, 200);
    //gfx_PrintStringXY("test2", 200, 100);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
}
#endif
void D3G_DrawPoint(Vector3 pos) 
{
    Vector2 Point = project(pos);
    gfx_SetPixel(Point.x, Point.y);
}
void D3G_DrawLine(Vector3 pos1, Vector3 pos2) 
{
    Vector2 Point1 = project(pos1);
    Vector2 Point2 = project(pos2);
    gfx_Line(Point1.x, Point1.y,Point2.x, Point2.y);
}
void D3G_DrawFunc() 
{

}
void D3G_Redraw() 
{

}
void D3G_Destroy()
{
    gfx_End();
}

Vector2 project(Vector3 point) {

    Vector2 screenPoint = {((point.x*fov)/(point.z+fov))+160, ((point.y*fov)/(point.z+fov))+120};
    /*
    * The equation in a Nutshell (Works for X and Y the same):
    *
    * X-------------|               We care just about the Ratios. The Picture on the left is just a weak example, but imagine it would work
    *  \            | 
    *   \           |   Z           PX         X
    *    \          |               ---  =  -------     | * FOV
    *     \         |               FOV     FOV + Z
    *      \---PX---|               
    *       \       |                       X * FOV
    *        \      |               PX   =  --------
    *         \     |                       FOV + Z
    *          \    |   FOV
    *           \   |
    *            \  |               As we can see, it is the same as on the line above.
    *             \ |               The +160 or +120 are just half of the screen Width/Height added on top, to center the projection.
    *              \|
    * 
    */
    return screenPoint;
}
