#include "GUInput.h"
//#include <ti/getcsc.h>
#include <ti/getkey.h>
#include <fileioc.h>
#include <ti/tokens.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include "GUI.h"
#include <string.h>

float getFloat(char * text) 
{
    float result = 0;
    size_t buffersize = 16;
    void * buf = malloc(buffersize*2);
    memset(buf, 0, buffersize*2);
        //if(buf == NULL) os_PutStrFull("Error malloc");
    os_GetTokenInput(text,buf,buffersize);
    os_Eval(buf,buffersize);
        //if(Error != 0) {os_PutStrFull("Error Evaling!"); char debug [10]; intToStr(Error,debug);os_PutStrFull(debug);}
    real_t temp;
    os_GetRealVar(OS_VAR_ANS, &temp);
        //if(Error != 0) os_PutStrFull("Error getting Var!");
        //os_GetKey();
    result = os_RealToFloat(&temp);
    free(buf);
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
    Vector3 result;
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
int16_d * MakeMenuList(GUIMenu * op1, GUIMenu* op2, GUIMenu * op3, GUIMenu * op4, GUIMenu * op5, uint8_t pos)
{
    int16_d retVal = 0;
    retVal.togther = 0;
start:
    uint8_t ret = 0;
    switch (pos) 
    {
        case 1: ret = MakeMenu(op1); break;
        case 2: ret = MakeMenu(op2); break;
        case 3: ret = MakeMenu(op3); break;
        case 4: ret = MakeMenu(op4); break;
        case 5: ret = MakeMenu(op5); break;
    }
    if (-1 > ret) 
    {
        pos = (pos + ret.val1 + 3) % 5;
        goto start;
    }
    retVal.val1 = pos;
    retVal.val2 = ret;
    return retVal.togther;
}

uint8_t MakeMenu(GUIMenu * menu) 
{
    #define X 170
    #define YOffset 9
    uint8_t CursorPos = 0;
    char t[2] = {64, '\0'};
    uint8_t betterY = YOffset;
    int MaxOptionRender = 0;
    int MinOptionRender = 0;
    int offset = 0;
generatingMainPart:
    ResetScreen();
    betterY = YOffset + CursorPos*11 - offset; 
    gfx_PrintStringXY(menu->title, 120, 1);
    gfx_PrintStringXY(t, X, betterY);
    RenderButtons("Exit", "", "", "", "");
    for (int i = MinOptionRender; i < menu->OptionsCount; i++) 
    {
        int y = YOffset + i*11-offset;
        if(y >= ButtomGUIBorder) 
        {
            MaxOptionRender = i;
            break;
        }        
        gfx_PrintStringXY(menu->Options[i],10, y);
        if(i < menu->ValueCount)
            gfx_PrintString(menu->Value[i]);
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
                if(CursorPos > menu->OptionsCount-1) {MaxOptionRender-=MinOptionRender;MinOptionRender = 0; CursorPos = 0;offset=0;}
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
            case sk_Right: return -2;
            case sk_Left: return -4;
            case sk_Mode:
            case sk_Del:
            //case sk_Clear: *exitPtrI = false; return -1; //Tf. Why do we want to exit the entire Programm?????
            /*case sk_Window: break;
            case sk_Zoom: break;
            case sk_Trace: break;
            case sk_Graph: break;*/
            default: continue;
        } 
        //gfx_PrintStringXY(&empty, X, betterY);
        gfx_SetColor(0xFF);//White, but Im too lazy to ignore this warning, so Im doing it manually
        gfx_FillRectangle(X, betterY,8,8);
        CursorPos %= menu->OptionsCount;
        betterY = YOffset + CursorPos*11 - offset; 
        //I hope the compiler compiles that Y good (5 + _ * 11); It does, thank you. But I did it manually anyway
        //Confused Hungo Bungos with the Comment above this one
        gfx_PrintStringXY(t, X, betterY);
    }
    return -0;
}
void InitGUIInput(bool * exitVar) 
{
    //exitPtrI = exitVar; //Tf. Why do we want to exit the entire Programm????? Anyway, not removing because maybe I need it and compiler will fix it anyway, trust!
}