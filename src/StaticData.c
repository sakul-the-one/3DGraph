#include <fileioc.h>
#include "StaticData.h"

void InitData()
{
    uint8_t file = ti_Open("D3GS","r"); //D3GS: 3DGraph Settings
    DataArray = malloc(sizeof(float)*5);
    ti_Read(DataArray, sizeof(float), 5, file);
    ti_Close(file);
}
void CloseData() 
{
    uint8_t file = ti_Open("D3GS","w"); //D3GS: 3DGraph Settings
    ti_Write(DataArray, sizeof(float), 5, file);
    ti_Close(file);
}
void SetDataArray(float * NewData) 
{
    for(int i = 0; i < 5; i++)
        DataArray[i] = NewData[i];
}
void SetDataValue(float NewData, uint8_t pos) 
{
    DataArray[pos] = NewData;
}
float * GetDataArray() 
{
    return DataArray;
}

/* //From GUI:
    So that we dont forget what each number does
    gfx_PrintStringXY("I forgot  ", 10, 5); gfx_PrintInt(data[0], 1);
    gfx_PrintStringXY("World X  ", 10, 16);gfx_PrintInt(data[1], 1);
    gfx_PrintStringXY("World Y  ", 10, 27);gfx_PrintInt(data[2], 1);
    gfx_PrintStringXY("World Z  ", 10, 38);gfx_PrintInt(data[3], 1);
    gfx_PrintStringXY("Details  ", 10, 49);gfx_PrintInt(data[4], 1);
*/