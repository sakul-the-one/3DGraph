#ifndef GUINPUT_H
#define GUINPUT_H
#include "D3/3DGraphics.h"
#define ButtomGUIBorder 195 //Buttons are 200, -5 to give some space

typedef struct 
{
 char * Title,
 char ** Options, 
 char ** Value,
 int OptionsCount, 
 int ValueCount   
} GUIMenu;
typedef union 
{
    uint8_t val1,
    uint8_t val2,
    uint16_t together
} int16_d;
bool startedInput = false;
bool * exitPtrI;
void InitGUIInput(bool * exitVar);
float startInputFloat(char * Prompt);
Vector3 startInputVector3();
uint8_t MakeMenu(char * Title,char ** Options, char ** Value ,int OptionsCount, int ValueCount);
int16_D MakeMenuList(GUIMenu * op1, GUIMenu* op2, GUIMenu * op3, GUIMenu * op4, GUIMenu * op5);
//float StringToFloat(char string[]);

#endif