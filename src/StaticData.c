#include <fileioc.h>
#include "StaticData.h"

void InitData()
{
    uint8_t file = ti_OpenVar("D3GS","r",0); //D3GS: 3DGraph Settings
    DataArray = malloc(sizeof(float)*5);
    ti_Read(DataArray, sizeof(float), 5, file);
    ti_Close(file);
}
void CloseData() 
{
    uint8_t file = ti_OpenVar("D3GS","w",0); //D3GS: 3DGraph Settings
    DataArray = malloc(sizeof(float)*5);
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