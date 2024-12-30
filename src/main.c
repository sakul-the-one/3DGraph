#include <ti/getkey.h>
#include <ti/getcsc.h>
#include <sys/timers.h>
#include "D3Handler.h"
#include "D3/3DGraphics.h"
#include "GUI.h"
#include "StaticData.h"


int main() 
{
  /*for (int i = 0; i<= 255; i++) 
  {
    char str = i;
    os_ClrHome();
    os_PutStrLine(&str);
    sleep(1);
  }*/
    Init();
    InitData();
    bool exit = true;
    bool needToRedraw = true;
    InitGUI(&exit, GetFunctionExsistsPointer());
    while (exit)
    {
      uint8_t key = os_GetCSC(); //I think GetKey is more Battary efficient :), But this would mean I need to research it, so fuck this. os_GetKey();
      switch (key)
      {
        case sk_Mode:
        case sk_Del:
        case sk_Clear: exit = false; break;
        case sk_Up: D3G_AddWorldRotation((Vector3){-22.5f,0,0}); needToRedraw = true; break; 
        case sk_Down: D3G_AddWorldRotation((Vector3){22.5f,0,0}); needToRedraw = true;break;
        case sk_Right: D3G_AddWorldRotation((Vector3){0,-22.5f,0}); needToRedraw = true;break;
        case sk_Left: D3G_AddWorldRotation((Vector3){0,22.5f,0}); needToRedraw = true; break;
        default: if(Input(key)) needToRedraw = true; break;
      }  
      if(needToRedraw) 
      {
        Redraw(); 
        //D3G_DrawCube((Vector3){10,25,20},20, (Vector3){0,0,0});
        needToRedraw = false;
      }
    }
    D3G_Destroy();
    CloseData();
    return 0;
}