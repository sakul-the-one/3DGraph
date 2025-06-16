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

#pragma region Init//First Return
void InitGUI(bool * exitVar) 
{
    exitPtr = exitVar;
    InitGUIInput(exitVar);
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
//void DrawEqu(int y);
void CreateVector3String(char **t, int which);
void CalcZ();
#pragma endregion

#pragma region AllMains
uint8_t MainFirst() //Turn specific equasion off. There is btw. a Bug when you press to much the Up_key, it will land at 5 instead of 0. Fixing this would take to much bytes imo and it is not neccessary...
{
    return 0b11;
}
uint8_t MainSecond() //Setting - like Word Position or Details...
{
    float * data = GetDataArray();
    float value = -3.25f;
    char **DataStr = malloc(5*sizeof(*DataStr));
MSstart: //GoTos are confusing... If I dont have a function above Variable declearios, there are errors...
    for (int i = 0; i < 5; i++) 
    {
        DataStr[i] = malloc(7);
        FloatToString(data[i],DataStr[i]);
    }
    int Selective = MakeMenu("",SettingsStrings,DataStr,5,5);
    if(Selective == -1)
        goto End;
    value = startInputFloat(SettingsStrings[Selective]);
    data[Selective] = value; 
    //Free:
End:
    for (int i = 0; i < 5; i++)  
        free(DataStr[i]);
    if(Selective != -1)
        goto MSstart;
    free(DataStr);
    return 0b11;
}
uint8_t MainThird() // Draw - like a Cube or so, although i would leave it empty for now, would use to much space...//EDIT: ITS THE MAIN THING FOR THE VECTOR VERSION
{
    char ** TextArray = NULL;
    char ** Optionsarray = NULL;
    int Result;
Medium:
    Optionsarray = malloc(26*sizeof(TextArray));
    TextArray = malloc(26*sizeof(TextArray));
    //char A = 'A';
    for(int i = 0; i<26; i++) 
    {
        char * Text = malloc(2);
        Text[0] = 'A'+i;
        Text[1] = '\0';
        TextArray[i] = Text;
        CreateVector3String(Optionsarray,i);
    }
    Result = MakeMenu("Add Vectors",TextArray, Optionsarray, 26,26);
    //gfx_PrintInt(Result,2);
    if (Result == -1)
        goto End;
    Vector3 MyVictorBuffer = startInputVector3();
    AddPoint(Result, MyVictorBuffer);
End:
    for(int i = 0; i<26; i++) 
    {
        free(TextArray[i]);
        free(Optionsarray[i]);
    }
    free(TextArray);
    free(Optionsarray);
    if (Result != -1)
        goto Medium;
    return 0b11;
}
uint8_t MainFourth() // Calc - To get the Z point f.e. or to find zero
{
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

void PrintCalc() 
{
    ResetArea();
    gfx_PrintStringXY("Calc Z", 10, 5);
}
void CreateVector3String(char **t, int which) 
{
    Vector3 * tmp = malloc(sizeof(Vector3));
    if(!GetPoint(which,tmp)) 
    {
        t[which] = malloc(1);
        (t[which])[0] = '\0';
        return;
    }
    char * result = malloc(26);//3*7 (for float) + 2 (fo Brackets) + 2 (for spaces) + 1 (for \0)= 26
    result[0] = '{';
    char * str = malloc(7);
    FloatToString(tmp->x,str);
    for (int i = 0; i < 7; i++)
        result[1+i] = str[i];
    free(str);
    result[8] = ' ';
    str = malloc(7);
    FloatToString(tmp->y,str);
    for (int i = 0; i < 7; i++)
        result[9+i] = str[i];
    free(str);
    result[16] = ' ';
    str = malloc(7);
    FloatToString(tmp->z,str);
    for (int i = 0; i < 7; i++)
        result[17+i] = str[i];
    free(str);
    result[24] = '}';
    for(int i = 0; i < 25; i++)
        if(result[i]==0) result[i] = ' ';
    result[25] = '\0';
    t[which] = result;
    free(tmp);
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
        int8_t mmmhh = (64 - Thickness)/2; //Today is the 14.06.2025... I just accidentally came back to this function... WTF IS `mmmhh`???
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
#pragma endregion
