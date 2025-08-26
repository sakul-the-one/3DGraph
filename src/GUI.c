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
#include <string.h>
#include <ti/getkey.h>

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

void ResetScreen();
#pragma endregion

#pragma region HelperFuctions
void ResetArea();
void DrawEqu(int y);
void PrintSettings(float * data);
void PrintCalc();
void CalcZ();
void CalcIntersectionLine();
void GFX_PrintFloat(float Value);
void FloatToString(float Value, char * str);
void CreateVector3String(char * result, Vector3 * vec);
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
    while (true) //I wish I could optimize that thingy here like in below, but this is actually that Optimized, that my optimisation might make it worse!
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
uint8_t MainFourth() // Calc - To get the Z point f.e. or to find zero; 
{
    ResetScreen();
    PrintCalc();
    RenderButtons("Exit", "", "", "", "");
    uint8_t CursorPos = 0;
    char t[2] = {64, '\0'};
    gfx_PrintStringXY(t, 160, 5);
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
        CursorPos %= 2;
        betterY = 5 + CursorPos*11; 
        PrintCalc();
        gfx_PrintStringXY(t, 160, betterY);
    }
Next:
    switch (CursorPos)
    {
        case 0: CalcZ(); break;
        case 1: CalcIntersectionLine(); break;
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
void ResetArea() //Dont Use ResetScreen if you are dealing with GUI: It will delete the Buttons below!
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_white);
    gfx_FillRectangle(0,5,170,120);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
}
void ResetScreen() 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_FillScreen(gfx_white);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
}
int SelectYVar(char * title) 
{
    int Selected = -1;
    int Pos = 0;
CZ_start:
    ResetArea();
    gfx_PrintStringXY(title, 1, 1);
    for(int i = 0, ii = 0; i<10; i++) 
    {
        uint8_t betterY = 10 + ii*11;    
        if(DoesFunctionExsist(i)) 
        {
            gfx_PrintStringXY("Y", 10, betterY);
            gfx_SetTextXY(18, betterY);
            gfx_PrintInt(i, 1);
            if(Pos == ii) Selected = i;
            ii++;
        }
    }
    int betterY = 10 + Pos * 11; 
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
            case sk_Clear: return -1;
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
    return Selected;
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
    Selected = SelectYVar("Select Y");
    if(Selected == -1) return;
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
    GFX_PrintFloat(zValue);
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
void CalcIntersectionLine() 
{
    int func1 = SelectYVar("Select first function:");
    if(func1 == -1) return;
    int func2 = SelectYVar("Select second function:");
    if(func2 == -1) return;
    //Create Vectors
    Vector3 * func1V = malloc(sizeof(Vector3));
    Vector3 * func2V = malloc(sizeof(Vector3));
    Vector3 * medium = malloc(sizeof(Vector3));
    Vector2 * X1 = malloc(sizeof(Vector2));
    Vector2 * X2 = malloc(sizeof(Vector2));
    Vector2 * X3 = malloc(sizeof(Vector2));
    Vector3 * AbsSolu = malloc(sizeof(Vector3));
    Vector3 * DirSolu = malloc(sizeof(Vector3));
    memset(func1V, 0 , sizeof(Vector3));
    memset(func2V, 0 , sizeof(Vector3));
    memset(medium, 0 , sizeof(Vector3));
    memset(AbsSolu, 0 , sizeof(Vector3));
    memset(DirSolu, 0 , sizeof(Vector3));
    memset(X1, 0 , sizeof(Vector2));
    memset(X2, 0 , sizeof(Vector2));
    memset(X3, 0 , sizeof(Vector2));
    //SET REAL ONES
    real_t zero = os_FloatToReal(0.0f);
    real_t one = os_FloatToReal(1.0f);
    //Set them to Zero
    os_SetRealVar(OS_VAR_X, &zero);
    os_SetRealVar(OS_VAR_Y, &zero);
    //Get&Set Function Values
    //Absolutes
    func1V->z = evaluateEquation(func1);
    func2V->z = evaluateEquation(func2);
    //Y War
    os_SetRealVar(OS_VAR_Y, &one);
    func1V->y = evaluateEquation(func1);
    func2V->y = evaluateEquation(func2);
    //X Var
    os_SetRealVar(OS_VAR_X, &one);
    os_SetRealVar(OS_VAR_Y, &zero);
    func1V->x = evaluateEquation(func1);
    func2V->x = evaluateEquation(func2);
        //func1V->z = 0x19531953;
    //Now that we have separeted them, we need to math it now.
    //Its Mathing time
        //printf("V: %f", func1V->x);
    char * res1 = malloc(26);
    char * res2 = malloc(26);
    CreateVector3String(res1, func1V);
    CreateVector3String(res2, func2V);
    
    //Printing Input (Why is 1 = 5 AND why no negative???)
    gfx_PrintStringXY(res1, 20,20); gfx_PrintInt(func1, 2);
    gfx_PrintStringXY(res2, 20,28); gfx_PrintInt(func2, 2);
    os_GetKey();
    //Making the In-Between Step:
    medium->x = func1V->x - func2V->x;
    medium->y = func1V->y - func2V->y;
    medium->z = func1V->z - func2V->z;
    //Creating The Solution:
    X1->x = (medium->z * -1) / medium->x;
    X1->y = (medium->y * -1) / medium->x;
    X2->x = 0;
    X2->y = 1;
    X3->x = (X1->x/medium->x) + medium->z;
    X3->y = (X1->y/medium->x) + medium->y;
    //Converting + Displaying:
    AbsSolu->x = X1->x;
    AbsSolu->y = X2->x;
    AbsSolu->z = X3->x;
    DirSolu->x = X1->y;
    DirSolu->y = X2->y;
    DirSolu->z = X3->y;
    char * AbsStr = malloc(26);
    char * DirStr = malloc(26);
    CreateVector3String(AbsStr,AbsSolu);
    CreateVector3String(DirStr,DirSolu);
    ResetScreen();
    gfx_PrintStringXY("Absolute: ", 10, 1); gfx_PrintString(AbsStr);
    gfx_PrintStringXY("Direction: ", 10,11); gfx_PrintString(DirStr);
    //Freeing
    free(res1); free(res2);
    free(func1V); free(func2V);
    free(X1); free(X2); free(X3);
    os_GetKey();
}
void PrintCalc() 
{
    ResetArea();
    gfx_PrintStringXY("Calc Z", 10, 5);
    gfx_PrintStringXY("Calc Intersection", 10, 16);
}
void PrintSettings(float * data) 
{
    ResetArea();
    gfx_PrintStringXY("I forgot  ", 10, 5); GFX_PrintFloat(data[0]);
    gfx_PrintStringXY("World X  ", 10, 16);GFX_PrintFloat(data[1]);
    gfx_PrintStringXY("World Y  ", 10, 27);GFX_PrintFloat(data[2]);
    gfx_PrintStringXY("World Z  ", 10, 38);GFX_PrintFloat(data[3]);
    gfx_PrintStringXY("Details  ", 10, 49);GFX_PrintFloat(data[4]);
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
void GFX_PrintFloat(float Value) 
{   
    char *str = malloc(7);
    FloatToString(Value, str);
    gfx_PrintString(str);
    free(str);
}
void FloatToString(float Value, char * str) 
{
    real_t *buf = malloc(sizeof(real_t));
    *buf = os_FloatToReal(Value);
    os_RealToStr(str, buf,6,4,2);
    free(buf);
}
void CreateVector3String(char * result, Vector3 * vec) 
{
    //char * result = malloc(26);//3*7 (for float) + 2 (fo Brackets) + 2 (for spaces) + 1 (for \0)= 26
    memset(result, ' ', 26);
    result[0] = '{';
    char * str = malloc(7);
    memset(str, '\0', 7);
    FloatToString(vec->x,str);
    for (int i = 0; i < 7; i++)
        result[1+i] = str[i];
    free(str);
    result[8] = ' ';
    str = malloc(7);
    memset(str, '\0', 7);
    FloatToString(vec->y,str);
    for (int i = 0; i < 7; i++)
        result[9+i] = str[i];
    free(str);
    result[16] = ' ';
    str = malloc(7);
    memset(str, '\0', 7);
    FloatToString(vec->z,str);
    for (int i = 0; i < 7; i++)
        result[17+i] = str[i];
    free(str);
    result[24] = '}';
    for(int i = 0; i < 25; i++)
        if(result[i]==0) result[i] = ' ';
    result[25] = '\0';
}
#pragma endregion
