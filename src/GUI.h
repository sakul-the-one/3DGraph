#ifndef GUI_H
#define GUI_H
#include <ti/getcsc.h>

typedef struct 
{
    uint16_t x;
    uint16_t y;
} Vector2Small;

uint8_t Gui;
bool * exitPtr;
bool Input(uint8_t key);
void InitGUI(bool * exitVar);
void RenderButtons(char * text1,char * text2,char * text3,char * text4,char * text5);
#endif