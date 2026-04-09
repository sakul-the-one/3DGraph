#include "D3/3DMath.h"
#include <math.h>

Vector3 D3_ADD(Vector3 v1, Vector3 v2)
{
    Vector3 ret;
    ret.x = v1.x + v2.x;
    ret.y = v1.y + v2.y;
    ret.z = v1.z + v2.z;
    return ret;
}
Vector3 D3_SUB(Vector3 v1, Vector3 v2)
{
    Vector3 ret;
    ret.x = v1.x - v2.x;
    ret.y = v1.y - v2.y;
    ret.z = v1.z - v2.z;
    return ret;
}
Vector3 D3_MUL(Vector3 v1, Vector3 v2)
{
    Vector3 ret;
    ret.x = v1.x * v2.x;
    ret.y = v1.y * v2.y;
    ret.z = v1.z * v2.z;
    return ret;
}
Vector3 D3_MULf(Vector3 v, float f)
{
    Vector3 ret;
    ret.x = v.x * f;
    ret.y = v.y * f;
    ret.z = v.z * f;
    return ret;
}
float D3_Skalar(Vector3 v1, Vector3 v2)
{
    float r;
    r = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return r;
}
Vector3 D3_CreateNormal(Vector3 v1, Vector3 v2)
{
    Vector3 ret;
    ret.x = v1.y * v2.z - v1.z * v2.y;
    ret.y = v1.z * v2.x - v1.x * v2.z;
    ret.z = v1.x * v2.y - v1.y * v2.x;
    return ret;
}
float D3_VectorDistance(Vector3 v1, Vector3 v2) 
{
    float SquaredDistance = (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z); //Vec Disntance^2 = sum((Vec1 - Vec2)^2)
    return sqrtf(SquaredDistance);
}