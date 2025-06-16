#include "GUInput.h"
//#include <ti/getcsc.h>
#include <ti/getkey.h>
#include <fileioc.h>
#include <ti/tokens.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include "GUI.h"

equ_t * equ;

float getFloat(char * text) 
{
    float result = 0;
    size_t buffersize = 15*sizeof(OS_TOK_RIGHT);
    equ = ti_MallocEqu(buffersize);
    os_GetTokenInput(text,&equ->data,buffersize);
    os_Eval(equ->data,equ->len);
    real_t temp;
    os_GetRealVar(OS_VAR_ANS, &temp);
    result = os_RealToFloat(&temp);
    free(equ);
    return result;
}
float startInputFloat(char * Prompt) 
{
    gfx_End();
    float result = getFloat(Prompt);
    gfx_Begin();
    return result;
}
Vector3 startInputVector3() 
{
    gfx_End();
    static Vector3 result;
    //os_GetKey();
    //Get X
    result.x = getFloat("X: ");
    os_NewLine();
    //Get Y
    result.y = getFloat("Y: ");
    os_NewLine();
    //Get Z
    result.z = getFloat("Z: ");
    os_NewLine();
    //Start gfx again and return result
    gfx_Begin();
    return result;
}
int MakeMenu(char * Title,char ** Options, char ** Value ,int OptionsCount, int ValueCount) 
{
    uint8_t CursorPos = 0;
    char t[2] = {64, '\0'};
    uint8_t betterY = 9;
    int MaxOptionRender = 0;
    int MinOptionRender = 0;
    int offset = 0;
generatingMainPart:
    ResetScreen();
    betterY = 9 + CursorPos*11 - offset; 
    gfx_PrintStringXY(Title, 120, 1);
    gfx_PrintStringXY(t, 120, betterY);
    RenderButtons("Exit", "", "", "", "");
    for (int i = MinOptionRender; i < OptionsCount; i++) 
    {
        int y = 8 + i*11-offset;
        if(y >= ButtomGUIBorder) 
        {
            MaxOptionRender = i;
            break;
        }        
        gfx_PrintStringXY(Options[i],10, y);
        if(i < ValueCount)
            gfx_PrintString(Value[i]);
    }

    while (true)
    {
        uint8_t key = os_GetCSC();  
        switch (key) 
        {
            case sk_Yequ: return -1;
            case sk_Down: 
                CursorPos++;
                if(MaxOptionRender == 0) break;
                if(CursorPos >= MaxOptionRender) {offset+=11;MinOptionRender++;MaxOptionRender++;}
                if(CursorPos > OptionsCount-1) {MaxOptionRender-=MinOptionRender;MinOptionRender = 0; CursorPos = 0;offset=0;}
                goto generatingMainPart;
                break;
            case sk_Up:
                CursorPos--;
                if(MaxOptionRender == 0) break;
                if(CursorPos<=MinOptionRender && CursorPos != 0 && MinOptionRender > 0){offset-=11;MinOptionRender--;MaxOptionRender--;}
                //if(MinOptionRender <=-1) {MinOptionRender = OptionsCount-MaxOptionRender +1;offset=(MinOptionRender)*11;MaxOptionRender = OptionsCount; CursorPos = OptionsCount;} ITS NOT WORKING SO IM NOT EVEN TRYING
                goto generatingMainPart;
                break;
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
        betterY = 9 + CursorPos*11 - offset; 
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