#ifndef D3HANDLER_H
#define D3HANDLER_H
#include <graphx.h>
#define border 30
#define ExtraBorder 20

void toggleFunction(uint8_t function);
void Redraw();
void DrawUI(bool redraw);
void Init();
void RedrawSaved();
bool is_bit_set(uint16_t value, int bit_position);
uint16_t toggle_bit(uint16_t byte, int bit_position);

#endif