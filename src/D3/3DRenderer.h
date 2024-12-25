#ifndef D3RENDERER_H
#define D3RENDERER_H
#include <graphx.h>
#include "D3/3DGraphics.h"
#include "3DGraphics.h"

typedef struct 
{
    Vector3 pos1;
    Vector3 pos2;
} DrawDataLine; 
//We dont need for Point, since it is just a Vector 2
bool D3G_SSD = false; //SaveScreenData

DrawDataLine * _DLD;//DrawLineData
//Vector3 * _DPD; //DrawPointData
int _DLDCount = 0;
int _DLDCountUsed = 0;
//float _lowestLD = 255.0f;
//int _DPDCount = 0;

//Not implemented, bc not needed (waste of space)void D3R_AddPoint(Vector3 pos);
void D3R_AddLine(Vector3 pos1, Vector3 pos2);
void D3R_PreMallocLine(int times);
void D3R_Clear();
void D3R_Draw();
#endif