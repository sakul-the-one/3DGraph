#include "GUI.h"
#include <sys/timers.h>
#include <fileioc.h>
#include <ti/getcsc.h>
#include <ti/tokens.h>
#include <graphx.h>
#include "D3Handler.h"
#include "GUI/GUInput.h"
#include "StaticData.h"
#include <math.h>

#pragma region Init
int8_t FR = 0b11; //First Return
void InitGUI(bool * exitVar, uint16_t * doesFunctionExsistPtr) 
{
    exitPtr = exitVar;
    DoesFunctionExsistPtr = doesFunctionExsistPtr;
    FR = 0b11;
}
uint8_t MainFirst();
uint8_t MainSecond(); 
uint8_t MainThird();
uint8_t MainFourth();
uint8_t MainFive();
#pragma endregion

#pragma region  ImportantFunctions
uint8_t Input(uint8_t key) 
{
    switch (key) 
    {
        case sk_Yequ: return MainFirst();
        case sk_Window: return MainSecond();
        case sk_Zoom: return MainThird();
        case sk_Trace: return MainFourth();
        case sk_Graph: return MainFive();
        //default: return FR;
    }
    if(FR != 0b00) { FR = 0b00; return 0b11;}
    return 0b00;
}

void ResetScreen() 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
}
#pragma endregion

#pragma region HelperFuctions
void ResetArea();
void DrawEqu(int y);
void PrintSettings(float * data);
void PrintCalc();
void CalcZ();
void GFX_PrintFloat(float Value, int afterpoint);
#pragma endregion

#pragma region AllMains
uint8_t MainFirst() //Turn specific equasion off. There is btw. a Bug when you press to much the Up_key, it will land at 5 instead of 0. Fixing this would take to much bytes imo and it is not neccessary...
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
            case sk_Yequ: return 0b11;
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
uint8_t MainSecond() //Setting - like Word Position or Details...
{
    float * data = GetDataArray();
    ResetScreen();
    RenderButtons("Exit", "", "", "", "");
    uint8_t CursorPos = 0;
    char t[2] = {64, '\0'};
    PrintSettings(data);
    gfx_PrintStringXY(t, 120, 5);
    uint8_t betterY = 5 + CursorPos*11;
    while (true)
    {
        float value = -3.25f;
        uint8_t key = os_GetCSC();  
        switch (key) 
        {
            case sk_Yequ: return 0b11;
            case sk_Down: CursorPos++;break;
            case sk_Up: CursorPos--;break;
            case sk_Enter: PrintSettings(data);value = *startInputFloat((Vector2){100, betterY}); break;
            case sk_Mode:
            case sk_Del:
            case sk_Clear: *exitPtr = false; return false;
            /*case sk_Window: break;
            case sk_Zoom: break;
            case sk_Trace: break;
            case sk_Graph: break;*/
            default: continue;
        } 
        CursorPos %= 5;
        if(value != -3.25f)
            data[CursorPos] = value;
        betterY = 5 + CursorPos*11; 
        //I hope the compiler compiles that Y good (5 + _ * 11); It does, thank you. But I did it manually anyway
        //Confused Hungo Bungos with the Comment above this one
        PrintSettings(data);
        gfx_PrintStringXY(t, 120, betterY);
    }
    return 0b11;
}
uint8_t MainThird() // Draw - like a Cube or so, although i would leave it empty for now, would use to much space...
{
    return 0b10;
}
uint8_t MainFourth() // Calc - To get the Z point f.e. or to find zero
{
    ResetScreen();
    PrintCalc();
    RenderButtons("Exit", "", "", "", "");
    uint8_t CursorPos = 0;
    char t[2] = {64, '\0'};
    gfx_PrintStringXY(t, 120, 5);
    uint8_t betterY = 5 + CursorPos*11;
    while (true)
    {
        uint8_t key = os_GetCSC();  
        switch (key) 
        {
            case sk_Yequ: return 0b11;
            case sk_Down: CursorPos++;break;
            case sk_Up: CursorPos--;break;
            case sk_Enter: goto Next; break;
            case sk_Mode:
            case sk_Del:
            case sk_Clear: *exitPtr = false; return false;
            /*case sk_Window: break;
            case sk_Zoom: break;
            case sk_Trace: break;
            case sk_Graph: break;*/
            default: continue;
        } 
        CursorPos %= 1;
        betterY = 5 + CursorPos*11; 
        gfx_PrintStringXY(t, 120, betterY);
    }
Next:
    switch (CursorPos)
    {
        case 0: CalcZ(); break;
        default: break;
    }
    return 0b10;
}
uint8_t MainFive() 
{ //Exit
    *exitPtr = false;
    return 0b00;
}
#pragma endregion


#pragma region  ImportantFunctionsInit
void ResetArea() 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_white);
    gfx_FillRectangle(0,5,130,120);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
}
void DrawEqu(int y) 
{
    ResetArea();
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
void CalcZ()
{
    int Selected = 0;
    int Pos = 0;
CZ_start:
    ResetArea();
    for(int i = 0, ii = 0; i<10; i++) 
    {
        uint8_t betterY = 5 + ii*11;    
        if(DoesFunctionExsist(i)) 
        {
            gfx_PrintStringXY("Y", 10, betterY);
            gfx_SetTextXY(18, betterY);
            gfx_PrintInt(i, 1);
            if(Pos == ii) Selected = i;
            ii++;
        }
    }
    int betterY = 5 + Pos * 11; 
    gfx_PrintStringXY("@", 120, betterY);
    while (true)
    {
        uint8_t key = os_GetCSC();  
        switch (key) 
        { 
            case sk_Down: Pos++;break;
            case sk_Up: Pos--;break;
            case sk_Enter: goto CZ_selected;break;
            case sk_Mode:
            case sk_Del:
            case sk_Yequ:
            case sk_Clear: return;
            /*case sk_Window: break;
            case sk_Zoom: break;
            case sk_Trace: break;
            case sk_Graph: break;*/
            default: continue;
        } 
        Pos %= 10;
        goto CZ_start;
    }
CZ_selected:
    ResetArea();
    gfx_PrintStringXY("x:",10,5);
    float * x = startInputFloat((Vector2){25, 5});
    gfx_PrintStringXY("y:",10,16);
    float * y = startInputFloat((Vector2){25, 16});

    real_t RealX = os_FloatToReal(*x);
    os_SetRealVar(OS_VAR_X, &RealX);

    real_t RealY = os_FloatToReal(*y);
    os_SetRealVar(OS_VAR_Y, &RealY);

    float zValue = evaluateEquation(Selected);
    //printf("%f", zValue); Debug
    gfx_PrintStringXY("z:",10,27);
    gfx_SetTextXY(25, 27);
    GFX_PrintFloat(zValue, 3);
    real_t RealAns = os_FloatToReal(zValue);
    os_SetRealVar(OS_VAR_ANS, &RealAns);
    while (true)
    {
        uint8_t key = os_GetCSC();  
        switch (key) 
        { 
            case sk_Enter: 
            case sk_Mode:
            case sk_Del:
            case sk_Yequ:
            case sk_Clear: return;
            default: continue;
        } 

    }
}
void PrintCalc() 
{
    ResetArea();
    gfx_PrintStringXY("Calc Z", 10, 5);
}
void PrintSettings(float * data) 
{
    ResetArea();
    gfx_PrintStringXY("I forgot  ", 10, 5); GFX_PrintFloat(data[0], 3);
    gfx_PrintStringXY("World X  ", 10, 16);GFX_PrintFloat(data[1], 3);
    gfx_PrintStringXY("World Y  ", 10, 27);GFX_PrintFloat(data[2], 3);
    gfx_PrintStringXY("World Z  ", 10, 38);GFX_PrintFloat(data[3], 3);
    gfx_PrintStringXY("Details  ", 10, 49);GFX_PrintFloat(data[4], 3);
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
#pragma endregion

#pragma region F2S
void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
 
// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning. 
int intToStr2(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
 
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
 
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
 
// Converts a floating-point/double number to a string. 
void ftoa(float n, char* res, int afterpoint) //Source https://www.geeksforgeeks.org/convert-floating-point-number-string/ btw.
{ 
    bool IsNegative = n < 0;
    if(IsNegative) n *= -1;
    // Extract integer part 
    int ipart = (int)n; 
 
    // Extract floating part 
    float fpart = n - (float)ipart; 
 
    // convert integer part to string 
    int i = intToStr2(ipart, res, 0); 
 
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
 
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
 
        intToStr2((int)fpart, res + i + 1, afterpoint); 
    }
    if (IsNegative) gfx_PrintString("-");
} 

void GFX_PrintFloat(float Value, int afterpoint) 
{
    char str[16];
    ftoa(Value, str ,afterpoint);
    gfx_PrintString(str);
}
#pragma endregion
