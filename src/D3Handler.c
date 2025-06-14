#include "D3Handler.h"
#include <fileioc.h>
#include "D3/3DGraphics.h"
#include "D3/3DRenderer.h"
#include "GUI.h"
#include "StaticData.h"

#define  max 10.0f
#define min -10.0f
#pragma region Vars

#pragma endregion

#pragma region BitOperations
// Function to toggle a specific bit
uint16_t toggle_bit(uint16_t byte, int bit_position) {
    // XOR the value with a mask where only the target bit is set
    byte ^= (1 << bit_position);
    return byte;
}
// Function to check if a specific bit is set
bool is_bit_set(uint16_t value, int bit_position) {
    // AND the value with a mask where only the target bit is set
    return (value & (1 << bit_position)) != 0;
}//*/
#pragma endregion
void AddCubeLines(Vector3 pos) 
{
    Vector3 vertices[8]; //D3G_RotatePoint(pos1, WorldRotation);
     // Front face vertices
    vertices[0].x = + pos.x; vertices[0].y = + pos.y; vertices[0].z = + pos.z;  // Top right front
    vertices[1].x = - pos.x; vertices[1].y = + pos.y; vertices[1].z = + pos.z;  // Top left front
    vertices[2].x = - pos.x; vertices[2].y = - pos.y; vertices[2].z = + pos.z;  // Bottom left front
    vertices[3].x = + pos.x; vertices[3].y = - pos.y; vertices[3].z = + pos.z;  // Bottom right front

    // Back face vertices
    vertices[4].x = + pos.x; vertices[4].y = + pos.y; vertices[4].z = - pos.z;  // Top right back
    vertices[5].x = - pos.x; vertices[5].y = + pos.y; vertices[5].z = - pos.z;  // Top left back
    vertices[6].x = - pos.x; vertices[6].y = - pos.y; vertices[6].z = - pos.z;  // Bottom left back
    vertices[7].x = + pos.x; vertices[7].y = - pos.y; vertices[7].z = - pos.z;  // Bottom right back

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
}
void AddPoint(uint8_t which, Vector3 value) 
{
    Points[which] = value;
    if(!is_bit_set(PointsSet, which)) toggle_bit(PointsSet,which);
}
void AddConnection(uint8_t pos1,uint8_t pos2) 
{
    LinkedLines * buf = malloc(sizeof(LinkedLines));
    if(first == NULL) first = buf;
    else last->next = buf;
    last = buf;
    buf->pos1 = pos1;
    buf->pos2 = pos2;
    buf->next = 0;
    LinkedListCount++;
}

void Redraw() //When it is true, it should be "normal"
{
    //Reset Screens (Ik, this block is ugly as fuck)
    LinkedLines * next = first;
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
    while (next != 0)
    {
        D3R_AddLine(Points[next->pos1],Points[next->pos2], 0x00);
        next = next->next;
    }
    DrawUI(false);//Old system, so idk. This will work!
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

void DrawUI(bool redraw) //When it is true, it should be "normal"
{
    float * Data = GetDataArray();
    //Horizontal Line is btw faster
    //Draw Square:
    //          x               y                           x              y
    //gfx_Line(border,        border,                     320-border,    border);                 //UL-UR
    gfx_HorizLine(border, border, 320 - (2*border));
    gfx_Line(border,        border,                     border,        240-border-ExtraBorder); //UL-DL
    gfx_Line(320-border,    border,                     320-border,    240-border-ExtraBorder); //UR-DR
    //gfx_Line(border,        240 -border -ExtraBorder,   320-border,    240-border-ExtraBorder); //DL-DR
    gfx_HorizLine(border,        240 -border -ExtraBorder, 320 - (2*border));

    const Vector3 TOP = {(Data[1] * 10),100 + (Data[2] * 10),(Data[3] * 10)};
    const Vector3 DOWN = {(Data[1] * 10),-100+(Data[2] * 10),(Data[3] * 10)};
    const Vector3 RIGHT = {100+(Data[1] * 10),(Data[2] * 10),(Data[3] * 10)};
    const Vector3 LEFT = {-100+(Data[1] * 10),(Data[2] * 10),(Data[3] * 10)};
    const Vector3 FORWARD = {(Data[1] * 10),(Data[2] * 10),100+(Data[3] * 10)};
    const Vector3 BACKWART = {(Data[1] * 10),(Data[2] * 10),-100+(Data[3] * 10)};

    //if(!redraw) return; //So if it isnt normal, it should not try to PreMalloc!

    D3R_PreMallocLine(3);
    D3R_AddLine(TOP, DOWN, 0);
    D3R_AddLine(RIGHT, LEFT, 0);
    D3R_AddLine(FORWARD, BACKWART, 0);

    D3G_DrawRotationCube((Vector2){140,-105});
}