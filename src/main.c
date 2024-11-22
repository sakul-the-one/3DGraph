#include <stdlib.h>
#include <ti/getkey.h>
#include <ti/vars.h>
#include "D3/3DGraphics.h"

int main() 
{
    D3G_Init();
    real_t x,y,z;
    os_GetMatrixElement(OS_VAR_MAT_I, 1,1, &x);
    os_GetMatrixElement(OS_VAR_MAT_I, 1,2, &y);
    os_GetMatrixElement(OS_VAR_MAT_I, 1,3, &z);
    Vector3 Point1 = {os_RealToFloat(&x),os_RealToFloat(&y),os_RealToFloat(&z)};
    Vector3 Point2 = {2,3,10};
    Vector3 Point3 = {5,10,1};
    D3G_DrawPoint(Point1);
    D3G_DrawLine(Point2, Point3);
    os_GetKey();
    D3G_Destroy();
    return 0;
}