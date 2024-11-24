#ifndef D3GRAPHICS_H
#define D3GRAPHICS_H
#define Debug true
#include <graphx.h>

typedef struct 
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct 
{
    float x;
    float y;
} Vector2;

typedef struct 
{
    Vector2 pos1;
    Vector2 pos2;
} DrawDataLine; //We dont need for Point, since it is just a Vector 2

int border = 30;
int ExtraBorder = 20; //For buttons
float fov = 90.0f;
//float aspectRatio = 320.0f / 240.0f;
//float near = 0.1f;
//float far = 100.0f;
bool D3G_SSD = false; //SaveScreenData

DrawDataLine * _DLD = NULL; //DrawLineData
Vector2 * _DPD = NULL; //DrawPointData
int _DLDCount = 0; //Remember to Count!
int _DPDCount = 0;

void D3G_Init();
void D3G_DrawPoint(Vector3 pos);
void D3G_DrawLine(Vector3 pos1,  Vector3 pos2);
void D3G_Destroy();
void D3G_DrawCube(Vector3 pos, int8_t size, Vector3 rotation);
void D3G_Redraw();
void D3G_SetSSD(bool value);
Vector3 D3G_RotatePoint(Vector3 point, Vector3 rotation);

#if Debug
void D3G_DrawDebugPoint(Vector3 pos);
void intToStr(int N, char *str);
#endif

Vector2 project(Vector3 point);
#endif