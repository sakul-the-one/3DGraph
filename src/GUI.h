#ifndef GUI_H
#define GUI_H
#include <ti/getcsc.h>

typedef struct 
{
    uint16_t x;
    uint16_t y;
} Vector2Small;
bool * exitPtr;
uint8_t Input(uint8_t key);
void InitGUI(bool * exitVar);
void ResetScreen();
void RenderButtons(char * text1,char * text2,char * text3,char * text4,char * text5);
char *SettingsStrings[] = {"Distant  ","World X  ","World Y  ","World Z  ","Details  "};
void FloatToString(float Value, char * str);
void GFX_PrintFloat(float Value);
#endif