#include <ti/getkey.h>
#include <ti/getcsc.h>
#include <sys/timers.h>
#include "D3Handler.h"
#include "D3/3DGraphics.h"
#include "GUI.h"
#include "StaticData.h"
#include "GUI/GUInput.h"
#include <stdio.h>

//int DebugMain();
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
    int8_t needToRedraw = 0b00;
    InitGUI(&exit);
    //DebugMain();
    while (exit)
    {
      uint8_t key = os_GetCSC(); //I think GetKey is more Battary efficient :), But this would mean I need to research it, so fuck this. os_GetKey();
      switch (key)
      {
        case sk_Mode:
        case sk_Del:
        case sk_Clear: exit = false; break;
        case sk_Up: D3G_AddWorldRotation((Vector3){-22.5f,0,0}); needToRedraw = 0b11; break; 
        case sk_Down: D3G_AddWorldRotation((Vector3){22.5f,0,0}); needToRedraw = 0b11;break;
        case sk_Right: D3G_AddWorldRotation((Vector3){0,-22.5f,0}); needToRedraw = 0b11;break;
        case sk_Left: D3G_AddWorldRotation((Vector3){0,22.5f,0}); needToRedraw = 0b11; break;
        default: needToRedraw = Input(key); break; //uint8_t
      }  
      if(needToRedraw > 0b01)
      {
        Redraw(); //00 = 0; 01 = 1; 10 = 2; 11 = 3; 100 = 4; 101 = 5; 110 = 6; 111 = 7
        D3G_DrawCube((Vector3){10,25,20},20, (Vector3){0,0,0});
      }
    }
    D3G_Destroy();
    CloseData();
    return 0;
}

#pragma region Debugs
void DebugLines(Vector2 pos1, Vector2 pos2);

int DebugMain() 
{ 
  DrawUI(true);
  DebugLines((Vector2){60,10}, (Vector2){80,35});
  DebugLines((Vector2){200,200}, (Vector2){170,180});
  DebugLines((Vector2){90,10}, (Vector2){70,50});
  DebugLines((Vector2){170,200}, (Vector2){200,180});
  DebugLines((Vector2){170,190}, (Vector2){200,180});
  DebugLines((Vector2){170,10}, (Vector2){200,190});
  DebugLines((Vector2){60,10}, (Vector2){200,200});
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
  gfx_FillScreen(gfx_white);
  gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
  return 0;
}
void DebugLines(Vector2 pos1, Vector2 pos2) 
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
    gfx_SetColor(gfx_black);
#pragma GCC diagnostic pop
gfx_Line(pos1.x, pos1.y, pos2.x, pos2.y);
  os_GetKey();
  D3G_ClipLine(&pos1, &pos2);
  #pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
  gfx_SetColor(gfx_red);
#pragma GCC diagnostic pop
  gfx_Line(pos1.x, pos1.y, pos2.x, pos2.y);
  os_GetKey();
}
#pragma endregion