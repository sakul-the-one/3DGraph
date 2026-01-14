#ifndef GUINPUT_H
#define GUINPUT_H
#include "D3/3DGraphics.h"
#define ButtomGUIBorder 195 //Buttons are 200, -5 to give some space

typedef struct 
{
 char * Title;
 char ** Options; 
 char ** Value;
 int OptionsCount; 
 int ValueCount;
} GUIMenu;
typedef struct 
{
    int8_t x;
    int8_t y;
} Vector2_16t;
typedef union 
{
    Vector2_16t val;
    uint16_t together;
} int16_d;
bool startedInput = false;
bool * exitPtrI;
void InitGUIInput(bool * exitVar);
float startInputFloat(char * Prompt);
Vector3 startInputVector3();
int8_t MakeMenu(GUIMenu * menu, bool reset); //Reset is on if not a MenuList. Very important difference!
int16_d MakeMenuList(GUIMenu * op1, GUIMenu* op2, GUIMenu * op3, GUIMenu * op4, GUIMenu * op5, uint8_t pos);
void freeGUIMenu(GUIMenu * menu,int StartingI);
//float StringToFloat(char string[]);

#endif