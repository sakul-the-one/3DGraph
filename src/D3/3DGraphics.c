#include "D3/3DGraphics.h"
#include <graphx.h>
#include <math.h>

#if Debug

#include <debug.h>
#define HAS_PRINTF = true;


float minMax(float min, float value, float max) 
{
    if(value < min) return min;
    else if(value > max) return max;
    else return value;
}

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

void D3G_SetWorldRotation(Vector3 NewRotation) 
{
    WorldRotation  = NewRotation;
}
Vector3 D3G_GetWorldRotation() 
{
    return WorldRotation;
}
void D3G_SetWorldPosition(Vector3 NewPosition) 
{
    WorldPosition  = NewPosition;
}
void D3G_AddWorldRotation(Vector3 NewRotation) 
{
    WorldRotation  = (Vector3){WorldRotation.x + NewRotation.x, WorldRotation.y + NewRotation.y, WorldRotation.z + NewRotation.z};
}
void D3G_AddWorldPosition(Vector3 NewPosition) 
{
    WorldPosition  = (Vector3){WorldPosition.x + NewPosition.x, WorldPosition.y + NewPosition.y, WorldPosition.z + NewPosition.z};
}
Vector3 D3G_GetWorldPosition() 
{
    return WorldPosition;
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
        (vertices[i]) = D3G_RotatePointNormalized(vertices[i], rotation, pos); //I'm lazy, so ToDo: Change it back to unnormalized and OPTIMIZE

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
    float sinX = sin(angle*DegreeToRadian); float cosX = cos(angle*DegreeToRadian);
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
    float sinY = sin(angle*DegreeToRadian); float cosY = cos(angle*DegreeToRadian);
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
    float sinZ = sin(angle*DegreeToRadian); float cosZ = cos(angle*DegreeToRadian); 
    return (Vector3)
    {
        point.x * cosZ - point.y * sinZ,
        point.x * sinZ + point.y * cosZ,
        point.z
    };
}
Vector3 D3G_RotatePointNormalized(Vector3 point, Vector3 rotation, Vector3 privot) 
{
    //Check D3G_RotatePoint(...) for a better explaination
    //this is just normalizing the position to 0 0 0, then Rotating and then giving back the position.
    //More Efficent would be for example to already decleare the normalized pos, rotate it and then put the position. 
    //With this approach, this function is not needable, but in some cases, having this function doesnt hurt.
    Vector3 NP = (Vector3){point.x - privot.x,point.y - privot.y,point.z - privot.z};
    if(rotation.x != 0) NP = RotateX(NP, rotation.x);
    if(rotation.y != 0) NP = RotateY(NP, rotation.y);
    if(rotation.z != 0) NP = RotateZ(NP, rotation.z);
    return (Vector3){NP.x + privot.x, NP.y - privot.y, NP.z - privot.z};
}
#if Debug
void D3G_DrawDebugPoint(Vector3 pos) 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_red);
#pragma GCC diagnostic pop
    Vector3 RotatedPos = D3G_RotatePoint(pos, WorldRotation);
    Vector2 Point = project(RotatedPos);
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
    Vector3 RotatedPos = D3G_RotatePoint(pos, WorldRotation);
    Vector2 Point = project(RotatedPos);
    gfx_SetPixel(Point.x, Point.y);
}
void D3G_DrawLine(Vector3 pos1, Vector3 pos2) 
{
    Vector3 RotatedPos1 = D3G_RotatePoint(pos1, WorldRotation);
    Vector3 RotatedPos2 = D3G_RotatePoint(pos2, WorldRotation);
    if(RotatedPos1.z < -fov && RotatedPos2.z < -fov) return;
    if (RotatedPos1.z < -fov) RotatedPos1.z = -fov;
    if (RotatedPos2.z < -fov) RotatedPos2.z = -fov;
    Vector2 Point1 = project(RotatedPos1);
    Vector2 Point2 = project(RotatedPos2);
    Point1.x = minMax(R3G_border, Point1.x, 320 - R3G_border);
    Point2.x = minMax(R3G_border, Point2.x, 320 - R3G_border);
    Point1.y = minMax(R3G_border, Point1.y, 240 - R3G_border - R3G_ExtraBorder);
    Point2.y = minMax(R3G_border, Point2.y, 240 - R3G_border - R3G_ExtraBorder);
    gfx_Line(Point1.x, Point1.y, Point2.x, Point2.y);
}
void D3G_DrawLineUnRotated(Vector3 pos1, Vector3 pos2) 
{
    if(pos1.z < -fov && pos2.z < -fov) return;
    if (pos1.z < -fov) pos1.z = -fov;
    if (pos2.z < -fov) pos2.z = -fov;
    Vector2 Point1 = project(pos1);
    Vector2 Point2 = project(pos2);
    Point1.x = minMax(R3G_border, Point1.x, 320 - R3G_border);
    Point2.x = minMax(R3G_border, Point2.x, 320 - R3G_border);
    Point1.y = minMax(R3G_border, Point1.y, 240 - R3G_border - R3G_ExtraBorder);
    Point2.y = minMax(R3G_border, Point2.y, 240 - R3G_border - R3G_ExtraBorder);
    gfx_Line(Point1.x, Point1.y, Point2.x, Point2.y);
}

void R3G_SetBorder(int _border,int _ExtraBorder) 
{
    R3G_border = _border;
    R3G_ExtraBorder = _ExtraBorder;
}
void D3G_Redraw() 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
}

void D3G_Destroy()
{
    gfx_End();
    /*Technically not needed, since 
    * 1. Programm closes probably after that
    * 2. No one will try to hack this and no one will be able to hack this, except the User
    * So if someone wants to save some Calculator Ram, delete these two lines: 
    */
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
