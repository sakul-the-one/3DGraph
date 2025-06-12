#include "GUInput.h"
//#include <ti/getcsc.h>
#include <ti/getkey.h>
#include <fileioc.h>
#include <ti/tokens.h>
#include <ti/screen.h>
#include <ti/getcsc.h>

equ_t * equ;

float  startInputFloat(char * Prompt) 
{
    gfx_End();
    size_t buffersize = 15*sizeof(OS_TOK_RIGHT);
    equ = ti_MallocEqu(buffersize);
    //os_GetKey();  
    os_GetTokenInput(Prompt,&equ->data,buffersize);
    float result = 0;
    os_Eval(equ->data,equ->len);
    real_t temp;
    os_GetRealVar(OS_VAR_ANS, &temp);
    result = os_RealToFloat(&temp);
    gfx_Begin();
    return result;
}
int MakeMenu(char ** Options, char ** Value ,int OptionsCount) 
{
    uint8_t CursorPos = 0;
    char t[2] = {64, '\0'};
    gfx_PrintStringXY(t, 120, 5);
    for (int i = 0; i < OptionsCount; i++) 
    {
        gfx_PrintStringXY(Options[i],10, 5 + i*11); gfx_PrintString(Value[i]);
    }
    uint8_t betterY = 5;
    while (true)
    {
        uint8_t key = os_GetCSC();  
        switch (key) 
        {
            case sk_Yequ: return -1;
            case sk_Down: CursorPos++;break;
            case sk_Up: CursorPos--;break;
            case sk_Enter: return CursorPos;
            case sk_Mode:
            case sk_Del:
            case sk_Clear: *exitPtrI = false; return false;
            /*case sk_Window: break;
            case sk_Zoom: break;
            case sk_Trace: break;
            case sk_Graph: break;*/
            default: continue;
        } 
        //gfx_PrintStringXY(&empty, 120, betterY);
        gfx_SetColor(0xFF);//White, but Im too lazy to ignore this warning, so Im doing it manually
        gfx_FillRectangle(120, betterY,8,8);
        CursorPos %= OptionsCount;
        betterY = 5 + CursorPos*11; 
        //I hope the compiler compiles that Y good (5 + _ * 11); It does, thank you. But I did it manually anyway
        //Confused Hungo Bungos with the Comment above this one
        gfx_PrintStringXY(t, 120, betterY);
    }
    return -0;
}
void InitGUIInput(bool * exitVar) 
{
    exitPtrI = exitVar;
}