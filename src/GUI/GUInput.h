#ifndef GUINPUT_H
#define GUINPUT_H
#include "D3/3DGraphics.h"

bool startedInput = false;
bool * exitPtrI;
void InitGUIInput(bool * exitVar);
float  startInputFloat(char * Prompt);
int MakeMenu(char * Title,char ** Options, char ** Value ,int OptionsCount, int ValueCount);
//float StringToFloat(char string[]);

#endif