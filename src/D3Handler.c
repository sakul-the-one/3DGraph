#include "D3Handler.h"
#include <fileioc.h>
#include "D3/3DGraphics.h"
#include "D3/3DRenderer.h"
#include "GUI.h"

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
void Init() 
{
    D3G_Init();
    R3G_SetBorder(border, ExtraBorder);
    //Redraw();
    //D3G_SSD = true; //Linker Error, apperently this doesn't work
    //D3G_SetSSD(true);
}
int FunctionsTrue = 10;

void DrawFunc() {
    float max = 10;
    float min = -10;
    float Pmin = min * -1;
    FunctionsTrue = 10;
    for (int i = 0; i < 10; i++)
        evaluateEquation(i);
    if (FunctionsTrue == 0) return;

    D3R_PreMallocLine((int)(Pmin * max * FunctionsTrue));
    for (float x = min; x < max; x++) {
        real_t RealX = os_FloatToReal(x);
        os_SetRealVar(OS_VAR_X, &RealX);
        float OldPoint[10] = {0}; 
        for (float y = min; y < max; y++) {
            real_t RealY = os_FloatToReal(y);
            os_SetRealVar(OS_VAR_Y, &RealY);
            for (int8_t i = 0; i < 10; i++) {   
                if (!DoesFunctionExsist(i)) continue;
                float zValue = evaluateEquation(i);
                if (y != min) {
                    D3R_AddLine(
                        (Vector3){(x - 1) * 10, (y - 1) * 10, OldPoint[i]}, 
                        (Vector3){x * 10, y * 10, zValue * 10}, i
                    );
                }
                OldPoint[i] = zValue * 10;
            }
        }
    }//After nestest Loop btw
}

float evaluateEquation(int_fast8_t which) {
    // Variables
    if(!DoesFunctionExsist(which)) 
    {
        FunctionsTrue--;
        return 0;
    }
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
        //FuntionExsists = toggle_bit(FuntionExsists, which);
        toggleFunction(which);
        FunctionsTrue--;
        return 0;
    }
}

void Redraw() 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    gfx_PrintStringXY("Calculating Next frame..",1,1);
    DrawUI();
    RenderButtons("Y=","Settings","WIP","Calc","Exit");
    DrawFunc();
    D3R_Draw();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_white);
    for (int i = 0; i<=8; i++)
        gfx_HorizLine(1,i,161);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    //D3R_Clear();
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

    const Vector3 TOP = {0,100,0};
    const Vector3 DOWN = {0,-100,0};
    const Vector3 RIGHT = {100,0,0};
    const Vector3 LEFT = {-100,0,0};
    const Vector3 FORWARD = {0,0,100};
    const Vector3 BACKWART = {0,0,-100};
    D3R_PreMallocLine(3);
    D3R_AddLine(TOP, DOWN, 0);
    D3R_AddLine(RIGHT, LEFT, 0);
    D3R_AddLine(FORWARD, BACKWART, 0);
}