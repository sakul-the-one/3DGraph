#ifndef D3HANDLER_H
#define D3HANDLER_H
#include <graphx.h>
#include "LinkedList.h"
#include "D3/3DGraphics.h"
#include "D3/3DRenderer.h"
#define border 30
#define ExtraBorder 20

uint32_t PointsSet = 0;
Vector3 Points[26];
float * Data = NULL;
LinkedList * Lines;
LinkedList * Layers;

void AddPoint(uint8_t which, Vector3 value);
bool GetPoint(uint8_t which, Vector3 * tmp);
uint32_t GetPointsSet();
void DeletePoint(uint8_t which);
bool IsVectorEmpty(Vector3 * vec);
LinkedList * GetLinesList();
LinkedList * GetPlanesList();
void toggleFunction(uint8_t function);
void Redraw();
void DrawUI(bool redraw);
void Init();
void RedrawSaved();
bool is_bit_set(uint32_t value, int bit_position);
void toggle_bit(uint32_t * byte, int bit_position);

#endif