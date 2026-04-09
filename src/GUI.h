#ifndef GUI_H
#define GUI_H
#include <ti/getcsc.h>
#include "GUI/GUInput.h"

typedef struct 
{
    uint16_t x;
    uint16_t y;
} Vector2Small;

static char CalcTitle[] = "Calc";
static char * CalcOptions[] = {"Calc Distance"};
static GUIMenu CalcMenu = (GUIMenu){CalcTitle, CalcOptions, NULL, 1, 0};

bool * exitPtr;
uint8_t Input(uint8_t key);
void InitGUI(bool * exitVar);
void ResetScreen();
const char *SettingsStrings[] = {"Distant  ","World X  ","World Y  ","World Z  ","Details  ", "Save Vectors", "Delete Saved"};
void FloatToString(float Value, char * str);
void GFX_PrintFloat(float Value);
static char EmptyStr[] = "";

#endif