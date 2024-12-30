#include "GUInput.h"
#include <ti/getcsc.h>

void MoveToTheRight(char * str, uint8_t pos, uint8_t max);

void SetPos(Vector2 pos) 
{
    position = pos;
}

float * startInputFloat() 
{
    if(startedInput) return NULL;
    startedInput = true;
    static float Result;
    char str[16] = {};
    uint8_t pos = 0;
    while (startedInput)
    {
        uint8_t key = os_GetCSC();  
        switch (key) 
        {
            case sk_Clear: for(uint8_t i = 0; i < 16; i++) str[i] = ' '; pos = 0; break;
            case sk_0: if(pos >= 16) break; str[pos] = '0'; pos++;break;
            case sk_1: if(pos >= 16) break; str[pos] = '1'; pos++;break;
            case sk_2: if(pos >= 16) break; str[pos] = '2'; pos++;break;
            case sk_3: if(pos >= 16) break; str[pos] = '3'; pos++;break;
            case sk_4: if(pos >= 16) break; str[pos] = '4'; pos++;break;
            case sk_5: if(pos >= 16) break; str[pos] = '5'; pos++;break;
            case sk_6: if(pos >= 16) break; str[pos] = '6'; pos++;break;
            case sk_7: if(pos >= 16) break; str[pos] = '7'; pos++;break;
            case sk_8: if(pos >= 16) break; str[pos] = '8'; pos++;break;
            case sk_9: if(pos >= 16) break; str[pos] = '9'; pos++;break;
            case sk_DecPnt: if(pos >= 16) break; str[pos] = '.'; pos++;break;
            case sk_Left: pos--; break;
            case sk_Right: pos++; break;
            case sk_Del: MoveToTheRight(str, pos, 16); break;
            case sk_Enter: startedInput = false;
            /*case sk_Window: break;
            case sk_Zoom: break;
            case sk_Trace: break;
            case sk_Graph: break;*/
            default: continue;
        }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_white);
    gfx_FillRectangle((int)position.x, (int)position.y, 80, 10);
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
    gfx_PrintStringXY(str, (int)position.x, (int)position.y);
    }
    //MakeNumber(&Result, &str, 16);
    Result = StringToFloat(str);
    return &Result;
}

void MoveToTheRight(char * str, uint8_t pos, uint8_t max) 
{
    for (uint8_t i = pos; i < max-1; i++) 
    {
        str[i] = str[i+1];
    }
}

int decimel = 0;
float multiplicator = 1;

float addInt(char CurrentChar) {
    float result = 0;
    switch (CurrentChar) {
        case '1': result += 1; break;
        case '2': result += 2; break;
        case '3': result += 3; break;
        case '4': result += 4; break;
        case '5': result += 5; break;
        case '6': result += 6; break;
        case '7': result += 7; break;
        case '8': result += 8; break;
        case '9': result += 9; break;
        case '0': result += 0; break;
        case '.': decimel = 1; break;
        case ',': decimel = 1; break;
        case '-': multiplicator *= -1; break;
        default:  break;
    }
    return result;
}

float addDecimal(char CurrentChar, int decimalPosition) {
    float result = 0;
    switch (CurrentChar) {
        case '1': result += 1; break;
        case '2': result += 2; break;
        case '3': result += 3; break;
        case '4': result += 4; break;
        case '5': result += 5; break;
        case '6': result += 6; break;
        case '7': result += 7; break;
        case '8': result += 8; break;
        case '9': result += 9; break;
        case '0': result += 0; break;
        default: break;
    }
    return result / (decimalPosition * 10);
}

float StringToFloat(char string[]) 
{
    //ressetting Varibales
    float result = 0;
    decimel = 0;
    multiplicator = 1;

    for (int i = 0; i < 16; i++) {
        if (string[i] == '\0') break;
        
        if (decimel == 0) {         
            float buffer = addInt(string[i]);
            if (decimel == 0)
                result = result * 10;
            result += buffer;
        } else {
            result += addDecimal(string[i], decimel);
            decimel++;
        }
    }
    return result * multiplicator;
}