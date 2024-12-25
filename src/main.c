#include <stdlib.h>
#include <ti/getkey.h>
#include <ti/vars.h>
#include <ti/getcsc.h>
#include "D3Handler.h"
#include "D3/3DGraphics.h"
#include <sys/timers.h>
#include <debug.h>

int main() 
{
    //D3G_Init();
    Init();
    bool exit = true;
    bool needToRedraw = true;
    while (exit)
    {
      uint8_t key = os_GetCSC(); //I think GetKey is more Battary efficient :), But this would mean I need to research it, so fuck this. os_GetKey();
      switch (key)
      {
        case sk_Clear: exit = false; break;
        //case 045: exit = false; break;
        case sk_Up: D3G_AddWorldRotation((Vector3){-22.5f,0,0}); needToRedraw = true; break; 
        case sk_Down: D3G_AddWorldRotation((Vector3){22.5f,0,0}); needToRedraw = true;break;
        case sk_Right: D3G_AddWorldRotation((Vector3){0,-22.5f,0}); needToRedraw = true;break;
        case sk_Left: D3G_AddWorldRotation((Vector3){0,22.5f,0}); needToRedraw = true; break;
        default: continue;
      }  
      if(needToRedraw) 
      {
        Redraw();
        D3G_DrawCube((Vector3){10,25,20},20, (Vector3){0,0,0});
        needToRedraw = false;
      }
    }
    D3G_Destroy();
    return 0;
}