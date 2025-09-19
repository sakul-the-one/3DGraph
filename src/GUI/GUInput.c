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
int16_d MakeMenuList(GUIMenu * op1, GUIMenu* op2, GUIMenu * op3, GUIMenu * op4, GUIMenu * op5, uint8_t pos)
{
    int16_d retVal;
    retVal.together = (uint16_t)0;
    int8_t ret = (uint8_t)0;
    int num = 0;
    if(op1 != NULL) num++; else return retVal;
    if(op2 != NULL) num++;
    if(op3 != NULL && num == 2) num++;
    if(op4 != NULL && num == 3) num++;
    if(op5 != NULL && num == 4) num++;
    retVal.val.x = (uint8_t)1;
    if(pos > num) return retVal; //This made some funny bugs btw
start:
    ret = 0;
    //Paint:
    ResetScreen();
    #pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_black);
        int xn = 64 * num;
    gfx_HorizLine(0, 40, xn);
    for (int i = 0; i < num; i++) 
    {  
        char * ptr;
        switch (i) 
        {
            case 0: ptr = op1->Title; break;
            case 1: ptr = op2->Title; break;
            case 2: ptr = op3->Title; break;
            case 3: ptr = op4->Title; break;
            case 4: ptr = op5->Title; break;
        }
        uint24_t x = 64*i;
        int Thickness = gfx_GetStringWidth(ptr);
        int8_t mmmhh = (64 - Thickness)/2; //Today is the 14.06.2025... I just accidentally came back to this function... WTF IS `mmmhh`???
    gfx_SetColor(gfx_white); //Today is the 19.9.2025... I just wrote a chemistry test and copied this code from the other file... WTF IS'mmmhh'????
        gfx_HorizLine(x-2, 40, 5);
    gfx_SetColor(gfx_black);
        gfx_PrintStringXY(ptr, x + mmmhh, 17); //mmmhh is the position difference between the line and Text. It is there, so the text is in the middle.
        gfx_SetPixel(x+1,39);gfx_SetPixel(x-1,39);
        gfx_Line(x, 38, x, 0);
    }
    gfx_SetColor(gfx_white);
        gfx_HorizLine(xn-2, 40, 5);
    gfx_SetColor(gfx_black);
        gfx_SetPixel(xn-1,39);
        gfx_Line(xn, 38, xn, 0);
    //Recolour the Selected one!
    gfx_SetColor(gfx_red);
    int xs = 64 * (pos-1);
        gfx_Line(xs,0,xs,38);
        gfx_HorizLine(xs, 40, 64);
        gfx_Line(xs+64,0,xs+64,38);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    //Paint End
    switch (pos) 
    {
        case 0: pos++; //if someone (like me) accidentally writes 0 instead of 1, then correct it and go to 1.
        case 1: ret = MakeMenu(op1, false); break;
        case 2: ret = MakeMenu(op2, false); break;
        case 3: ret = MakeMenu(op3, false); break;
        case 4: ret = MakeMenu(op4, false); break;
        case 5: ret = MakeMenu(op5, false); break;

        default: pos = 0; goto start;
    }
    if (-1 > ret) 
    {
        pos = (pos + ret + 3) % (num+1);
        goto start;
    }
    retVal.val.x = pos;
    retVal.val.y = ret;
    return retVal;
}

int8_t MakeMenu(GUIMenu * menu, bool reset) 
{
    #define X 170
    int YOffset = 9;
    uint8_t CursorPos = 0;
    char t[2] = {64, '\0'};
    uint8_t betterY = YOffset;
    int MaxOptionRender = 0;
    int MinOptionRender = 0;
    int offset = 0;
generatingMainPart:
    if(reset)
        ResetScreen();
    else 
    {
        YOffset = 45;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_white);
    gfx_FillRectangle(0,41,320,ButtomGUIBorder-35);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    }
    betterY = YOffset + CursorPos*11 - offset; 
    if(reset)
        gfx_PrintStringXY(menu->Title, 120, 1);
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
            case sk_Right: if(!reset) return -2; break;
            case sk_Left: if(!reset) return -4; break;
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
    return -1;
}
void InitGUIInput(bool * exitVar) 
{
    //exitPtrI = exitVar; //Tf. Why do we want to exit the entire Programm????? Anyway, not removing because maybe I need it and compiler will fix it anyway, trust!
}