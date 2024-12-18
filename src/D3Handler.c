#include "D3Handler.h"
#include <fileioc.h>
#include "D3/3DGraphics.h"

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
}

void Init() 
{
    D3G_Init();
    Redraw();
    //D3G_SSD = true; //Linker Error, apperently this doesn't work
    //D3G_SetSSD(true);
}

void DrawFunc() 
{
   //evaluate(1);
   //D3G_DrawPoint((Vector3){0,0,1});
   for (float x = -10; x < 0; x++) 
   {
        real_t RealX = os_FloatToReal(x);
        os_SetRealVar(OS_VAR_X, &RealX);
        for (float y = -10; y < 0; y++) 
        {
            real_t RealY = os_FloatToReal(y);
            os_SetRealVar(OS_VAR_Y, &RealY);
            for (int i = 0; i< 10; i++) 
            {
                if(!is_bit_set(FuntionExsists, i)) continue;
                float zValue = evaluateEquation(i);
                D3G_DrawPoint((Vector3){x*10, y*10, zValue*10});
            }
        }
   }
}

float evaluateEquation(int_fast8_t which) {
    // Variables
    equ_t *equation;
    float value;
    switch (which)
    {
        case 0: equation = os_GetEquationData(OS_VAR_Y0, 0); break;
        case 1: equation = os_GetEquationData(OS_VAR_Y1, 0); break;//os_GetEquationData(ti_GetTokenString(OS_TOK_EQU_Y1, sizeof(OS_TOK_EQU_Y1), NULL), 0); break;
        case 2: equation = os_GetEquationData(OS_VAR_Y2, 0); break;
        case 3: equation = os_GetEquationData(OS_VAR_Y3, 0); break;
        case 4: equation = os_GetEquationData(OS_VAR_Y4, 0); break;
        case 5: equation = os_GetEquationData(OS_VAR_Y5, 0); break;
        case 6: equation = os_GetEquationData(OS_VAR_Y6, 0); break;
        case 7: equation = os_GetEquationData(OS_VAR_Y7, 0); break;
        case 8: equation = os_GetEquationData(OS_VAR_Y8, 0); break;
        case 9: equation = os_GetEquationData(OS_VAR_Y9, 0); break;
        default:
            return 0;
    }

    if (equation->len >= 1) //Checks if equation exsists
    {
        os_Eval(equation->data, equation->len);
        real_t temp;
        os_GetRealVar(OS_VAR_ANS, &temp);
        //os_SetRealVar(OS_VAR_A, &temp);
        value = os_RealToFloat(&temp);
        return value;
    } else {
        // If Y does not Exsist, flip the value
        toggle_bit(FuntionExsists, which);
        return 0;
    }
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
}

void DrawUI() 
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