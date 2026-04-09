#ifndef D3MATH_H
#define D3MATH_H
#include "D3/3DGraphics.h"

Vector3 D3_ADD(Vector3 v1, Vector3 v2);
Vector3 D3_SUB(Vector3 v1, Vector3 v2);
Vector3 D3_MUL(Vector3 v1, Vector3 v2);
Vector3 D3_MULf(Vector3 v, float f);
float D3_Skalar(Vector3 v1, Vector3 v2);
Vector3 D3_CreateNormal(Vector3 v1, Vector3 v2);
float D3_VectorDistance(Vector3 v1, Vector3 v2);
#endif