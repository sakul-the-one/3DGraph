#include "D3Handler.h"
#include "D3/3DGraphics.h"


void Init() 
{
    D3G_Init();
    Redraw();
    //D3G_SSD = true; //Linker Error, apperently this doesn't work
    D3G_SetSSD(true);
}

void DrawFunc() 
{

}
void RedrawSaved() 
{
    D3G_Redraw();
}
void Redraw() 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    DrawUI();
    DrawFunc();
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