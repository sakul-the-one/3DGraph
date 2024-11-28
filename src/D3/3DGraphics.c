#include "D3/3DGraphics.h"
#include <graphx.h>
#include <math.h>
#define PI 3.14159265358979323846

#if Debug
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
#endif

void D3G_Init() 
{
    gfx_Begin();
}

void D3G_DrawCube(Vector3 pos, int8_t size, Vector3 rotation) 
{
    //D3G_Redraw();
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
    //Front face
    D3G_DrawLine(vertices[0], vertices[1]);
    D3G_DrawLine(vertices[1], vertices[2]);
    D3G_DrawLine(vertices[2], vertices[3]);
    D3G_DrawLine(vertices[3], vertices[0]);

    //Back face
    D3G_DrawLine(vertices[4], vertices[5]);
    D3G_DrawLine(vertices[5], vertices[6]);
    D3G_DrawLine(vertices[6], vertices[7]);
    D3G_DrawLine(vertices[7], vertices[4]);

    //Connecting edges
    D3G_DrawLine(vertices[0], vertices[4]);
    D3G_DrawLine(vertices[1], vertices[5]);
    D3G_DrawLine(vertices[2], vertices[6]);
    D3G_DrawLine(vertices[3], vertices[7]);
}
Vector3 RotateX(Vector3 point, float angle);
Vector3 RotateY(Vector3 point, float angle);
Vector3 RotateZ(Vector3 point, float angle);

Vector3 D3G_RotatePoint(Vector3 point, Vector3 rotation) {
    /*
    * rotation Matrixes:
    *      [1        0        0     ]
    * RX = [0 cos(angle) -sin(angle)]
    *      [0 sin(angle)  cos(angle)]
    *
    *      [cos(angle)  0 sin(angle)]
    * RY = [0           1     0     ]
    *      [-sin(angle) 0 cos(angle)]
    *
    *      [cos(angle) -sin(angle) 0]
    * RZ = [sin(angle) cos(angle)  0]
    *      [0           0          1] 
    *
    * Example Usage: 
    * Rotated Point X:
    * NP.x = OP.x * 1 + OP.y *0 +OP.z *0 // NP = new Point; OP = Old Point
    * NP.y = OP.x * 0 + OP.y * cos(angle) + OP.z * -sin(angle)
    * NP.z = OP.x * 0 + OP.y * sin(angle) + OP.z * cos(angle)
    *
    * How Matrix Multplation works:
    *         OP.x   OP.y        OP.z
    * NP.x = [1     0           0     ]
    * NP.y = [0   cos(angle) -sin(angle)]
    * NP.z = [0   sin(angle)  cos(angle)]
    *
    * Now, normally you would use a Matrix Multiplication function, but since we need to
    * do it for every Point, it will cost a lot of space (that we don't have) or a lot of
    * Ram (that we also don't have). So in order to work carefully with the Resources we have,
    * I will put everthing basicly in one line and check if we need even to calculate all of it.
    */

    //Decleres return Variable
    Vector3 NP = point;
    //Check for every Rotation and if it Rotates, then apply Rotation, just as explained before
    if(rotation.x != 0) NP = RotateX(NP, rotation.x);
    if(rotation.y != 0) NP = RotateY(NP, rotation.y);
    if(rotation.z != 0) NP = RotateZ(NP, rotation.z);
    return NP;
    
}
//trust me, it will be more efficent that way. although not that readable...   
//Explaination: we mathematically exclude the multiplication, so that we just have to do it one time. 
Vector3 RotateX(Vector3 point, float angle) 
{
    /*
    *      [1        0        0     ]
    * RX = [0 cos(angle) -sin(angle)]
    *      [0 sin(angle)  cos(angle)]
    */
    float sinX = sin(angle); float cosX = cos(angle);
    return (Vector3)
    {
        point.x,
        point.y * cosX - point.z * sinX,
        point.y * sinX + point.z * cosX
    };
}
Vector3 RotateY(Vector3 point, float angle) 
{
    /*
    *      [cos(angle)  0 sin(angle)]
    * RY = [0           1     0     ]
    *      [-sin(angle) 0 cos(angle)]
    */
    float sinY = sin(angle); float cosY = cos(angle);
    return (Vector3)
    { 
        point.x * cosY + point.z * sinY,
        point.y,
        -point.x * sinY + point.z * cosY
    };
}
Vector3 RotateZ(Vector3 point, float angle) 
{
    /*
    *      [cos(angle) -sin(angle) 0]
    * RZ = [sin(angle) cos(angle)  0]
    *      [0           0          1]
    */
    float sinZ = sin(angle); float cosZ = cos(angle); 
    return (Vector3)
    {
        point.x * cosZ - point.y * sinZ,
        point.x * sinZ + point.y * cosZ,
        point.z
    };
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
    gfx_PrintStringXY("test3", 200, 100);
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

    if (D3G_SSD) 
    {        
        Vector2 * temp = malloc(sizeof(Vector2)*(_DPDCount+1));

        for (int i = 0; i < _DPDCount; i++)
            temp[i] = _DPD[i];
        temp[_DPDCount] = Point; 

        if(_DPD != NULL)
            free(_DPD);
        _DPD = temp;
        _DPDCount++;
    }
}
void D3G_DrawLine(Vector3 pos1, Vector3 pos2) 
{
    Vector2 Point1 = project(pos1);
    Vector2 Point2 = project(pos2);
    gfx_Line(Point1.x, Point1.y,Point2.x, Point2.y);

    if (D3G_SSD) 
    {
        DrawDataLine * temp = malloc(sizeof(DrawDataLine)*(_DLDCount+1));

        DrawDataLine _temp = {Point1, Point2};
        for (int i = 0; i < _DLDCount; i++)
            temp[i] = _DLD[i];
        temp[_DLDCount] = _temp; 

        if(_DLD != NULL)
            free(_DLD);
        _DLD = temp;
       _DLDCount++;
    }
}

void D3G_Redraw() 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop

    for (int i = 0; i < _DLDCount; i++)
        gfx_Line(_DLD[i].pos1.x, _DLD[i].pos1.y,_DLD[i].pos2.x, _DLD[i].pos2.y);

    for (int i = 0; i < _DPDCount; i++)
        gfx_SetPixel(_DPD[i].x,_DPD[i].y);
}

void D3G_SetSSD(bool value) 
{
    D3G_SSD = value;
}
void D3G_Destroy()
{
    free(_DLD);
    free(_DPD);
    gfx_End();
    /*Technically not needed, since 
    * 1. Programm closes probably after that
    * 2. No one will try to hack this and no one will be able to hack this, except the User
    * So if someone wants to save some Calculator Ram, delete these two lines: 
    */
    _DLD = NULL;
    _DPD = NULL;
}

Vector2 project(Vector3 point) {

    Vector2 screenPoint = {((point.x*fov)/(point.z+fov))+160, ((point.y*fov)/(point.z+fov))+120};
    /*
    * The equation in a Nutshell (Works for X and Y the same):
    *
    * \-------X-----|               We care just about the Ratios. The Picture on the left is just a weak example, but imagine it would work
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
