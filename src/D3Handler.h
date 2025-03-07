#ifndef D3HANDLER_H
#define D3HANDLER_H
#include <graphx.h>


int border = 30;
int ExtraBorder = 20; //For buttons

uint16_t FuntionExsists = 0b0000001111111111; 
uint16_t * GetFunctionExsistsPointer();

bool DoesFunctionExsist(uint8_t function);
void toggleFunction(uint8_t function);
void Redraw(bool redraw);
void DrawUI(bool redraw);
void Init();
void RedrawSaved();
void CalcFunc();
float evaluateEquation(int_fast8_t which);
bool is_bit_set(uint16_t value, int bit_position);
uint16_t toggle_bit(uint16_t byte, int bit_position);

#endif