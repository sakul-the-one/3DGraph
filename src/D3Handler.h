#ifndef D3HANDLER_H
#define D3HANDLER_H
#include <graphx.h>
#include "D3/3DGraphics.h"
#include "D3/3DRenderer.h"
#define border 30
#define ExtraBorder 20

typedef struct
{
    uint8_t pos1;
    uint8_t pos2;
    void *next;
} LinkedLines;

uint32_t PointsSet = 0;
Vector3 Points[26];
LinkedLines * first = NULL;
LinkedLines * last = NULL;
int LinkedListCount = 0;

void AddPoint(uint8_t which, Vector3 value);
void AddConnection(uint8_t pos1,uint8_t pos2);
void toggleFunction(uint8_t function);
void Redraw();
void DrawUI(bool redraw);
void Init();
void RedrawSaved();
bool is_bit_set(uint16_t value, int bit_position);
uint16_t toggle_bit(uint16_t byte, int bit_position);

#endif