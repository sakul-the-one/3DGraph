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
int border = 30;
int ExtraBorder = 20; //For buttons
float fov = 90.0f;
float aspectRatio = 320.0f / 240.0f;
float near = 0.1f;
float far = 100.0f;

void D3G_Init();
void DrawUI();
void D3G_DrawPoint(Vector3 pos);
void D3G_DrawLine(Vector3 pos1,  Vector3 pos2);
void D3G_Redraw();
void D3G_Destroy();
void D3G_DrawCube(Vector3 pos, int8_t size, Vector3 rotation);
Vector3 D3G_RotatePoint(Vector3 point, Vector3 rotation);

#if Debug
void D3G_DrawDebugPoint(Vector3 pos);
#endif

Vector2 project(Vector3 point);
#endif