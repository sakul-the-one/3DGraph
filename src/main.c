#include <stdlib.h>
#include <ti/getkey.h>
#include <ti/vars.h>
#include <ti/getcsc.h>
#include "D3/3DGraphics.h"
#include "D3Handler.h"
#include <sys/timers.h>


int main() 
{
    //D3G_Init();
    Init();
    D3G_SetSSD(false);
    real_t x,y,z;
    real_t rx,ry,rz;
    os_GetMatrixElement(OS_VAR_MAT_I, 1,1, &x);
    os_GetMatrixElement(OS_VAR_MAT_I, 1,2, &y);
    os_GetMatrixElement(OS_VAR_MAT_I, 1,3, &z);
    os_GetMatrixElement(OS_VAR_MAT_I, 2,1, &rx);
    os_GetMatrixElement(OS_VAR_MAT_I, 2,2, &ry);
    os_GetMatrixElement(OS_VAR_MAT_I, 2,3, &rz);
    Vector3 Point1 = {os_RealToFloat(&x),os_RealToFloat(&y),os_RealToFloat(&z)};
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

    char rxt[16];
    char ryt[16];
    char rzt[16];

    while (!os_GetCSC())
    {
        i+=5;   
        sleep(1);
        RedrawSaved();
        D3G_DrawCube(Point1, 25, (Vector3){i,i*2,i*4});
        if (i >= 90*4) i=0;
      ///*  
        /*RedrawSaved();
        i+=5;
        rot.x = i%90; //For normal stuff 360, BUT a Cube is the same, every 90° Degrees!
        rot.y = (i*2)%90;
        rot.z = (i*4)%90;
        intToStr((int)rot.x, &rxt);
        gfx_PrintStringXY(rxt, 10, 10);

        intToStr((int)rot.y, &ryt);
        gfx_PrintStringXY(rxt, 10, 20);

        intToStr((int)rot.z, &rzt);
        gfx_PrintStringXY(rxt, 10, 30);

        D3G_DrawCube(Point1, 25, rot);
        if (i >= 90*4) i=0;
        sleep(1); */
    }
    RedrawSaved();//*/ }
    //sleep(1);
    D3G_Destroy();
    return 0;
}