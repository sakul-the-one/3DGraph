#include "D3Handler.h"
#include <fileioc.h>
#include "D3/3DGraphics.h"
#include "D3/3DRenderer.h"
#include "D3/3DMath.h"
#include "StaticData.h"
#include "GUI.h"
#include "StaticData.h"

#define  max 10.0f
#define min -10.0f
#pragma region Vars

#pragma endregion

#pragma region BitOperations
// Function to toggle a specific bit
void toggle_bit(uint32_t *byte, int bit_position) {
    // XOR the value with a mask where only the target bit is set
    *byte ^= (1 << bit_position);
    //eturn byte;
}
// Function to check if a specific bit is set
bool is_bit_set(uint32_t value, int bit_position) {
    // AND the value with a mask where only the target bit is set
    return (value & (1 << bit_position)) != 0;
}//*/
#pragma endregion

void AddCubeLines(Vector3 pos) 
{
    Vector3 vertices[8]; //D3G_RotatePoint(pos1, WorldRotation);
    #define halfsize 5
    //Move the Point:
    pos.x += Data[1];
    pos.y += Data[2];
    pos.z += Data[3];
    //Distance: Standart: 10
    pos.x *= Data[0];
    pos.y *= Data[0];
    pos.z *= Data[0];
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
    D3R_PreMallocLine(12);
    D3R_AddLine((Vector3){vertices[0].x,vertices[0].y,vertices[0].z}, (Vector3){vertices[1].x, vertices[1].y,vertices[1].z}, 0x00);
    D3R_AddLine((Vector3){vertices[1].x,vertices[1].y,vertices[1].z}, (Vector3){vertices[2].x, vertices[2].y,vertices[2].z}, 0x00);
    D3R_AddLine((Vector3){vertices[2].x,vertices[2].y,vertices[2].z}, (Vector3){vertices[3].x, vertices[3].y,vertices[3].z}, 0x00);
    D3R_AddLine((Vector3){vertices[3].x,vertices[3].y,vertices[3].z}, (Vector3){vertices[0].x, vertices[0].y,vertices[0].z}, 0x00);
    //Back Face
    D3R_AddLine((Vector3){vertices[4].x,vertices[4].y,vertices[4].z}, (Vector3){vertices[5].x, vertices[5].y,vertices[5].z}, 0x00);
    D3R_AddLine((Vector3){vertices[5].x,vertices[5].y,vertices[5].z}, (Vector3){vertices[6].x, vertices[6].y,vertices[6].z}, 0x00);
    D3R_AddLine((Vector3){vertices[6].x,vertices[6].y,vertices[6].z}, (Vector3){vertices[7].x, vertices[7].y,vertices[7].z}, 0x00);
    D3R_AddLine((Vector3){vertices[7].x,vertices[7].y,vertices[7].z}, (Vector3){vertices[4].x, vertices[4].y,vertices[4].z}, 0x00);
    //Connecting edges
    D3R_AddLine((Vector3){vertices[0].x,vertices[0].y,vertices[0].z}, (Vector3){vertices[4].x, vertices[4].y,vertices[4].z}, 0x00);
    D3R_AddLine((Vector3){vertices[1].x,vertices[1].y,vertices[1].z}, (Vector3){vertices[5].x, vertices[5].y,vertices[5].z}, 0x00);
    D3R_AddLine((Vector3){vertices[2].x,vertices[2].y,vertices[2].z}, (Vector3){vertices[6].x, vertices[6].y,vertices[6].z}, 0x00);
    D3R_AddLine((Vector3){vertices[3].x,vertices[3].y,vertices[3].z}, (Vector3){vertices[7].x, vertices[7].y,vertices[7].z}, 0x00);
}
void Init() 
{
    D3G_Init();
    Data = GetDataArray();
    //int multiplicator = GetDataArray()[0];
    if(Data[0] <= 0) 
    {
        Data[0] = 10;
        //SetDataValue(10,0); //BROTHER! ITS ALL THE SAME POINTER. THATS DOPPELT GEMOPELT!
    }
    for(int i = 0; i<26; i++) 
    {
        Points[i] = (Vector3){0,0,0};
    }
    Lines = CreateList();
    Layers = CreateList();
}
void AddPoint(uint8_t which, Vector3 value) 
{
    Points[which] = value;
    if(!is_bit_set(PointsSet, which)) toggle_bit(&PointsSet,which);
}
uint32_t GetPointsSet()
{
    return PointsSet;
}
bool GetPoint(uint8_t which, Vector3 * tmp)
{
    *tmp = Points[which];
    return is_bit_set(PointsSet, which);
}

LinkedList * GetLinesList()
{
    return Lines;
}
LinkedList * GetPlanesList() 
{
    return Layers;
}

void Redraw() //When it is true, it should be "normal"
{
    //Reset Screens (Ik, this block is ugly as fuck)
    LinkedItem * nextLine = Lines->first;
    LinkedItem * nextPlane = Layers->first;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop //Ressetting and putting new Data
    D3R_Clear();
    RenderButtons(" ","Settings","Add","Calc","Exit");
    gfx_PrintStringXY("Generating Next frame..",1,1);
    for (int i = 0; i< 27; i++) 
    {
        if(is_bit_set(PointsSet,i)) 
            AddCubeLines(Points[i]);
    }
    D3R_PreMallocLine(Lines->count);
    //Draws Layers/Planes. I know, that I use them interchangeable. Whoops
    while (nextLine != NULL)
    {
        int* pData = nextLine->Data; //Pls love C!
        Vector3 p1 = Points[pData[0]];
        Vector3 p2 = Points[pData[1]];
        //Move the Point:
        p1.x += Data[1];
        p1.y += Data[2];
        p1.z += Data[3];
        p2.x += Data[1];
        p2.y += Data[2];
        p2.z += Data[3];
        //Distance: Standart: 10
        p1 = D3_MULf(p1, Data[0]);
        p2 = D3_MULf(p2, Data[0]);
        D3R_AddLine(p1,p2, 0x00);
        nextLine = nextLine->next;
    }
    int colour = 0;
    float step = 1/Data[4];
    D3R_PreMallocLine(Layers->count * (Data[4] + 2));//Function for premallocing lines: Data[4] * Layers->count + Layers->count * 2 <=> Layers->count * (Data[4] + 2) //Distributionsgesetzt Motherfucker :)
    while (nextPlane != NULL)
    {
        //Get All Points
        int* pData = nextPlane->Data; //Pls love C!
        Vector3 p1 = Points[pData[0]];
        Vector3 p2 = Points[pData[1]];
        Vector3 p3 = Points[pData[2]];
        //Richtungsvektor = Intimidate
        Vector3 i1,i2;
        i1 = D3_SUB(p2, p1);
        i2 = D3_SUB(p3, p1);
        //Draw:
        D3R_AddLine(p1,p2, colour);
        D3R_AddLine(p1,p3, colour);
        for (float x = 0; x < 1; x += step)
        { 
            //First line:
            Vector3 ndp1 = D3_ADD(p1, D3_MULf(i1, x));
            ndp1.x += Data[1];
            ndp1.y += Data[2];
            ndp1.z += Data[3];
            //Second Line
            Vector3 ndp2 = D3_ADD(p1, D3_MULf(i2, x));
            ndp2.x += Data[1];
            ndp2.y += Data[2];
            ndp2.z += Data[3];
            //Distance: Standart: 10
            ndp1 = D3_MULf(ndp1, Data[0]);
            ndp2 = D3_MULf(ndp2, Data[0]);
            //Connect those lines :)
            D3R_AddLine(ndp1, ndp2, colour);
        }
        nextPlane = nextPlane->next;
        colour++;
    }

    DrawUI(true);//Old system, so idk. This will work!
    D3R_Draw(true);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_white);
    for (int i = 0; i<=8; i++)
        gfx_HorizLine(1,i,161);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    //D3R_Clear();
}

void DrawUI(bool redraw) //When it is true, it should be "normal"//If it is false, it will return!
{
    //Horizontal Line is btw faster
    //Draw Square:
    //          x               y                           x              y
    //gfx_Line(border,        border,                     320-border,    border);                 //UL-UR
    gfx_HorizLine(border, border, 320 - (2*border));
    gfx_Line(border,        border,                     border,        240-border-ExtraBorder); //UL-DL
    gfx_Line(320-border,    border,                     320-border,    240-border-ExtraBorder); //UR-DR
    //gfx_Line(border,        240 -border -ExtraBorder,   320-border,    240-border-ExtraBorder); //DL-DR
    gfx_HorizLine(border,        240 -border -ExtraBorder, 320 - (2*border));

    if(!redraw) return; //So if it isnt normal, it should not try to PreMalloc! //If it is false, it will return!

    const Vector3 TOP = {(Data[1] * 10),100 + (Data[2] * 10),(Data[3] * 10)};
    const Vector3 DOWN = {(Data[1] * 10),-100+(Data[2] * 10),(Data[3] * 10)};
    const Vector3 RIGHT = {100+(Data[1] * 10),(Data[2] * 10),(Data[3] * 10)};
    const Vector3 LEFT = {-100+(Data[1] * 10),(Data[2] * 10),(Data[3] * 10)};
    const Vector3 FORWARD = {(Data[1] * 10),(Data[2] * 10),100+(Data[3] * 10)};
    const Vector3 BACKWART = {(Data[1] * 10),(Data[2] * 10),-100+(Data[3] * 10)};

    D3R_PreMallocLine(3);
    D3R_AddLine(TOP, DOWN, 0);
    D3R_AddLine(RIGHT, LEFT, 0);
    D3R_AddLine(FORWARD, BACKWART, 0);

    D3G_DrawRotationCube((Vector2){140,-105});
}
