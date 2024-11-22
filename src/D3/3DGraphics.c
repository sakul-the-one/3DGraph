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
    D3G_Redraw();
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

void D3G_DrawCube(Vector3 pos, int8_t size, Vector3 rotation) 
{
    D3G_Redraw();
    float halfsize = size /2.f;
    Vector3 vertices[8];
     // Front face vertices
    vertices[0].x = pos.x + halfsize; vertices[0].y = pos.y + halfsize; vertices[0].z = pos.z + halfsize;  // Top right front
    vertices[1].x = pos.x - halfsize; vertices[1].y = pos.y + halfsize; vertices[1].z = pos.z + halfsize;  // Top left front
    vertices[2].x = pos.x - halfsize; vertices[2].y = pos.y - halfsize; vertices[2].z = pos.z + halfsize;  // Bottom left front
    vertices[3].x = pos.x + halfsize; vertices[3].y = pos.y - halfsize; vertices[3].z = pos.z + halfsize;  // Bottom right front

    // Back face vertices
    vertices[4].x = pos.x + halfsize; vertices[4].y = pos.y + halfsize; vertices[4].z = pos.z - halfsize;  // Top right back
    vertices[5].x = pos.x - halfsize; vertices[5].y = pos.y + halfsize; vertices[5].z = pos.z - halfsize;  // Top left back
    vertices[6].x = pos.x - halfsize; vertices[6].y = pos.y - halfsize; vertices[6].z = pos.z - halfsize;  // Bottom left back
    vertices[7].x = pos.x + halfsize; vertices[7].y = pos.y - halfsize; vertices[7].z = pos.z - halfsize;  // Bottom right back

    //Rotate these suckers: 
    for (int i = 0; i < 8; i++)
        (vertices[i]) = D3G_RotatePoint(vertices[i], rotation);

    //Draw them
    // Draw edges
    // Front face
    D3G_DrawLine(vertices[0], vertices[1]);
    D3G_DrawLine(vertices[1], vertices[2]);
    D3G_DrawLine(vertices[2], vertices[3]);
    D3G_DrawLine(vertices[3], vertices[0]);

    // Back face
    D3G_DrawLine(vertices[4], vertices[5]);
    D3G_DrawLine(vertices[5], vertices[6]);
    D3G_DrawLine(vertices[6], vertices[7]);
    D3G_DrawLine(vertices[7], vertices[4]);

    // Connecting edges
    D3G_DrawLine(vertices[0], vertices[4]);
    D3G_DrawLine(vertices[1], vertices[5]);
    D3G_DrawLine(vertices[2], vertices[6]);
    D3G_DrawLine(vertices[3], vertices[7]);
}

Vector3 D3G_RotatePoint(Vector3 point, Vector3 rotation) {
    Vector3 rotated = point;

    // Rotate around X-axis
    float cosX = cos(rotation.x);
    float sinX = sin(rotation.x);
    rotated.y = point.y * cosX - point.z * sinX;
    rotated.z = point.y * sinX + point.z * cosX;

    // Rotate around Y-axis
    float cosY = cos(rotation.y);
    float sinY = sin(rotation.y);
    rotated.x = point.x * cosY + point.z * sinY;
    rotated.z = -point.x * sinY + point.z * cosY;

    // Rotate around Z-axis
    float cosZ = cos(rotation.z);
    float sinZ = sin(rotation.z);
    rotated.x = point.x * cosZ - point.y * sinZ;
    rotated.y = point.x * sinZ + point.y * cosZ;

    return rotated;
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    void DrawUI();
    D3G_DrawFunc();
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
