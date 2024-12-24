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
    DrawFunc();
    bool exit = true;
    bool needToRedraw = true;
    while (exit)
    {
      uint8_t key = os_GetCSC();
      switch (key)
      {
        case sk_Clear: exit = false; break;
        case sk_Up: D3G_AddWorldRotation((Vector3){-22.5f,0,0}); needToRedraw = true; break;
        case sk_Down: D3G_AddWorldRotation((Vector3){22.5f,0,0}); needToRedraw = true;break;
        case sk_Right: D3G_AddWorldRotation((Vector3){0,-22.5f,0}); needToRedraw = true;break;
        case sk_Left: D3G_AddWorldRotation((Vector3){0,22.5f,0}); needToRedraw = true; break;
        default: continue;
      }  
      if(needToRedraw) 
      {
        DrawFunc(); 
        needToRedraw = false;
      }
    }
    D3G_Destroy();
    return 0;
}