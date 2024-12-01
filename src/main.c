#include <stdlib.h>
#include <ti/getkey.h>
#include <ti/vars.h>
#include <ti/getcsc.h>
#include "D3Handler.h"
#include "D3/3DGraphics.h"
#include <sys/timers.h>

int main() 
{
    //D3G_Init();
    Init();
    D3G_SetSSD(false);
    //DrawFunc();
    real_t x,y,z;
    real_t rx,ry,rz;
    os_GetMatrixElement(OS_VAR_MAT_I, 1,1, &x);
    os_GetMatrixElement(OS_VAR_MAT_I, 1,2, &y);
    os_GetMatrixElement(OS_VAR_MAT_I, 1,3, &z);
    os_GetMatrixElement(OS_VAR_MAT_I, 2,1, &rx);
    os_GetMatrixElement(OS_VAR_MAT_I, 2,2, &ry);
    os_GetMatrixElement(OS_VAR_MAT_I, 2,3, &rz);
    Vector3 Point1 = {os_RealToFloat(&x),os_RealToFloat(&y)*-1,os_RealToFloat(&z)};
    Vector3 rot = {os_RealToFloat(&rx),os_RealToFloat(&ry),os_RealToFloat(&rz)};
    Vector3 Point2 = {2,3,10};
    Vector3 Point3 = {5,10,1};
    D3G_DrawDebugPoint(Point1);
    D3G_DrawLine(Point2, Point3);
    //os_GetKey();
    int i = 0;
    //Vector3 pos = {10,50,80};
    //Vector3 rot ={0,0,0};
    D3G_DrawCube(Point1, 25, rot);
    D3G_SetSSD(false);
    bool exit = true;
    while (exit)
    {
      uint8_t key = os_GetCSC();
      switch (key)
      {
        case sk_Clear: exit = false; break;
        case sk_Up: D3G_AddWorldRotation((Vector3){-22.5f,0,0}); break;
        case sk_Down: D3G_AddWorldRotation((Vector3){22.5f,0,0}); break;
        case sk_Right: D3G_AddWorldRotation((Vector3){0,-22.5f,0}); break;
        case sk_Left: D3G_AddWorldRotation((Vector3){0,22.5f,0}); break;
      }  
      i+=5;   
      sleep(1);
      RedrawSaved();
      D3G_DrawCube(Point1, 25, (Vector3){i,i*2,i*4});
      D3G_DrawDebugPoint(Point1);
      D3G_DrawDebugPoint((Vector3){0,0,0});
      if (i >= 90*4) i=0;
    }
    RedrawSaved();
    D3G_Destroy();
    return 0;
}