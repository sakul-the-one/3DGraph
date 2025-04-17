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
uint16_t * GetFunctionExsistsPointer() 
{
    return &FuntionExsists;
}
bool DoesFunctionExsist(uint8_t function) 
{
    return (FuntionExsists & (1 << function)) != 0;
}
void toggleFunction(uint8_t function) 
{
    FuntionExsists ^= (1 << function);
}
#pragma endregion

void Init() 
{
    D3G_Init();

}


void Redraw() //When it is true, it should be "normal"
{
    //Reset Screens (Ik, this block is ugly as fuck)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    D3R_Clear();
    RenderButtons("Y=","Settings"," ","Calc","Exit");
    gfx_PrintStringXY("Calculating Next frame..",1,1);
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

    if(!redraw) return; //So if it isnt normal, it should not try to PreMalloc!

    D3R_PreMallocLine(3);
    D3R_AddLine(TOP, DOWN, 0);
    D3R_AddLine(RIGHT, LEFT, 0);
    D3R_AddLine(FORWARD, BACKWART, 0);

    D3G_DrawRotationCube((Vector2){140,-105});
}