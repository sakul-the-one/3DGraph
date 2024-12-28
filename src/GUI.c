#include "GUI.h"
#include <sys/timers.h>
#include <ti/getcsc.h>
#include <ti/tokens.h>
#include <fileioc.h>
#include <graphx.h>
#include "D3Handler.h"

void InitGUI(bool * exitVar, uint16_t * doesFunctionExsistPtr) 
{
    exitPtr = exitVar;
    DoesFunctionExsistPtr = doesFunctionExsistPtr;
}
bool MainFirst();
bool MainSecond(); 
bool MainThird();
bool MainFourth();
bool MainFive();
bool Input(uint8_t key) 
{
    switch (key) 
    {
        case sk_Yequ: return MainFirst();
        case sk_Window: return MainSecond();
        case sk_Zoom: return MainThird();
        case sk_Trace: return MainFourth();
        case sk_Graph: return MainFive();
        default: return false;
    }
    return true;
}
void ResetScreen() 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
}
void DrawEqu(int y);

bool MainFirst() //Turn specific equasion off. There is btw. a Bug when you press to much the Up_key, it will land at 5 instead of 0. Fixing this would take to much bytes imo and it is not neccessary...
{
//Shower
//U need too btw ;)
    ResetScreen();
    RenderButtons("Exit", "", "", "", "");
    DrawEqu(5);
    uint8_t CursorPos = 0;
    char t[2] = {64, '\0'};
    gfx_PrintStringXY(t, 120, 5);
    while (true)
    {
        uint8_t key = os_GetCSC();   
        switch (key) 
        {
            case sk_Yequ: return true;
            case sk_Down: CursorPos++;break;
            case sk_Up: CursorPos--;break;
            case sk_Enter: *DoesFunctionExsistPtr = toggle_bit(*DoesFunctionExsistPtr, CursorPos); break;
            case sk_Mode:
            case sk_Del:
            case sk_Clear: *exitPtr = false; return false;
            /*case sk_Window: break;
            case sk_Zoom: break;
            case sk_Trace: break;
            case sk_Graph: break;*/
            default: continue;
        }
        DrawEqu(5);
        CursorPos %= 10;
        uint8_t betterY = 5 + CursorPos*11;
        gfx_PrintStringXY(t, 120, betterY);
    }
}
bool MainSecond() //Setting - like Word Position or Details...
{
    return true;
}
bool MainThird() // Draw - like a Cube or so, although i would leave it empty for now, would use to much space...
{
    return true;
}
bool MainFourth() // Calc - To get the Z point f.e. or to find zero
{
    return true;
}
bool MainFive() 
{ //Exit
    *exitPtr = false;
    return false;
}
void DrawEqu(int y) 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_white);
    gfx_FillRectangle(0,5,130,120);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    for(int i = 0; i<10; i++) 
    {
        /*equ_t *equation;
        switch (i)
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
            default:return;
        }
        unsigned int str_lengh = 16;
        char * ptr = ti_GetTokenString(&equation->data, &equation->len, NULL);   
        gfx_PrintStringXY(ptr, 10, y + i*10);//*/
        uint8_t betterY = y + i*11;
        gfx_PrintStringXY("Y   = ", 10, betterY);
        if(DoesFunctionExsist(i)) gfx_PrintString("on"); else gfx_PrintString("off");
        gfx_SetTextXY(18, betterY);
        gfx_PrintInt(i, 1);
    }
}
void RenderButtons(char * text1,char * text2,char * text3,char * text4,char * text5) 
{
    int num = 0;
    if(text1[0] != '\0') num++; else return;
    if(text2[0] != '\0') num++;
    if(text3[0] != '\0' && num == 2) num++;
    if(text4[0] != '\0' && num == 3) num++;
    if(text5[0] != '\0' && num == 4) num++;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_black);
        int xn = 64 * num;
    gfx_HorizLine(0, 200, xn);
    for (int i = 0; i < num; i++) 
    {  
        char * ptr;
        switch (i) 
        {
            case 0: ptr = text1; break;
            case 1: ptr = text2; break;
            case 2: ptr = text3; break;
            case 3: ptr = text4; break;
            case 4: ptr = text5; break;
        }
        uint24_t x = 64*i;
        int Thickness = gfx_GetStringWidth(ptr);
        int8_t mmmhh = (64 - Thickness)/2;
    gfx_SetColor(gfx_white);
        gfx_HorizLine(x-2, 200, 5);
    gfx_SetColor(gfx_black);
        gfx_PrintStringXY(ptr, x + mmmhh, 215);
        gfx_SetPixel(x+1,201);gfx_SetPixel(x-1,201);
        gfx_Line(x, 202, x, 240);
    }
    gfx_SetColor(gfx_white);
        gfx_HorizLine(xn-2, 200, 5);
    gfx_SetColor(gfx_black);
        gfx_SetPixel(xn-1,201);
        gfx_Line(xn, 202, xn, 240);
#pragma GCC diagnostic pop
}