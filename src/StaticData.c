#include "StaticData.h"
#include <fileioc.h>
#include <string.h>
#include "D3/3DRenderer.h"
#include "LinkedList.h"
#include "D3Handler.h"

#define HEADER_SIZE 3

void InitData()
{
    uint8_t file = ti_Open("D3GS","r"); //D3GS: 3DGraph Settings
    DataArray = malloc(sizeof(float)*5);
    if(file == 0)
        memset(DataArray, 0, sizeof(float)*5);
    else 
    {
        ti_Read(DataArray, sizeof(float), 5, file);
        ti_Close(file);
    }
}
void CloseData() 
{
    uint8_t file = ti_Open("D3GS","w"); //D3GS: 3DGraph Settings
    ti_Write(DataArray, sizeof(float), 5, file);
    ti_Close(file);
    free(DataArray);
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
//Saving Vectors:
void SaveVectorData() 
{
    D3R_Clear(); //Clear Ram, so we have more space in Ram :)
    uint8_t file = ti_Open("D3SV","w"); //D3SV: 3DGraph Save Vectors
    if (!file) return; //Should never happen, as we delete old & create new
    LinkedList * Lines = GetLinesList();
    LinkedList * Planes = GetPlanesList();
    size_t mallocSize = sizeof(float) * 78 + (Lines->count * 2 + Planes->count * 3 + HEADER_SIZE) * sizeof(int); //78 for 26 letters * 3 floats
    uint8_t * DataArray = malloc(mallocSize); //Create Array
    if (DataArray == NULL) 
    {
        gfx_PrintStringXY("Error saving! RAM too full",0,0);
        return;
    }
    ((int*)DataArray)[0] = (int)mallocSize; //safe the size
    ((int*)DataArray)[1] = Lines->count;
    ((int*)DataArray)[2] = Planes->count;
    uint8_t * newDataPointer = DataArray + sizeof(int) * HEADER_SIZE;
    for (int i = 0; i < 26; i++) //26 for every letter
    {
        Vector3 p;
        GetPoint(i, &p);
        ((float*)newDataPointer)[i * 3 + 0] = p.x;
        ((float*)newDataPointer)[i * 3 + 1] = p.y;
        ((float*)newDataPointer)[i * 3 + 2] = p.z;
    }
    newDataPointer += sizeof(float) * 26 * 3; //Normally there would be i instead of 26, but i will always be 26
    //Save Lines:
    int n = 0;
    int i = 0;
    LinkedItem * item = Lines->first;
    while (n < Lines->count)
    {
        int * data = item->Data;
        for (int j = 0; j < item->DataLenght; j++) 
        {
            ((int*)newDataPointer)[i] = data[j];
            i++;  
        }
        item = item->next;
        n++;
    }
    //Save Planes:
    n = 0;
    item = Planes->first;
    while (n < Planes->count)
    {
        int * data = item->Data;
        for (int j = 0; j < item->DataLenght; j++) 
        {
            ((int*)newDataPointer)[i] = data[j];
            i++;
        }
        item = item->next;
        n++;
    }
    //Write&Close
    ti_Write(DataArray, mallocSize, 1, file); //yes. It should not be made like this. But I made it like this. This is a Datadump.
    ti_Close(file);
    free(DataArray);
}
int LoadVectorData() 
{
    uint8_t file = ti_Open("D3SV","r"); //D3GS: Save Vectors
    if (file == 0) return 0; //If failed, ignore it. Probably doesnt exsists then
    int fileSize = 0;
    size_t error = ti_Read(&fileSize, sizeof(int), 1, file);
    if (error == 0) 
    {
        gfx_PrintStringXY("Failed to read Header",0,0);
        ti_Close(file);
        ti_Delete("D3SV");
        return 2;
    }
    uint8_t * DataArray = malloc(fileSize);
    if (DataArray == 0) { //Not enough Ram, ignore it again!
        gfx_PrintStringXY("Error Making Space! RAM too full",0,0);
        ti_Close(file);
        ti_Delete("D3SV");
        return 3;
    }
    ti_Close(file);
    file = ti_Open("D3SV","r"); //D3GS: Save Vectors
    error = ti_Read(DataArray, fileSize, 1, file);
    if (error == 0) 
    {
        gfx_PrintStringXY("Failed to read complete File",0,0);
        ti_Close(file);
        ti_Delete("D3SV");
        return 4;
    }
    //Actually Parsing the Data now :)
    int LinesCount = ((int*)DataArray)[1];
    int PlanesCount = ((int*)DataArray)[2];
    uint8_t * newDataPointer = DataArray + sizeof(int) * HEADER_SIZE;
    LinkedList * Lines = GetLinesList();
    LinkedList * Layers = GetPlanesList();
    LinkedItem * item = NULL;
    //All Points
    for (int i = 0; i < 26; i++) 
    {
        Vector3 p;
        p.x = ((float*)newDataPointer)[i * 3 + 0];
        p.y = ((float*)newDataPointer)[i * 3 + 1];
        p.z = ((float*)newDataPointer)[i * 3 + 2];
        if(!IsVectorEmpty(&p))
            AddPoint(i, p);
    }
    newDataPointer += sizeof(float) * 26 * 3;
    //Add all Lines
    item = Lines->first;
    for (int i = 0, n = 0; i < LinesCount; i++) 
    {
        int * data = malloc(sizeof(int) * 2);
        data[0] = ((int*)newDataPointer)[n];
        n++;
        data[1] = ((int*)newDataPointer)[n];
        n++;
        AddItem(Lines, data, 2);
        item = item->next;
    }
    //Add All Planes
    item = Layers->first;
    newDataPointer += sizeof(int) * 2 * LinesCount;
    for (int i = 0, n = 0; i < PlanesCount; i++)
    {
        int * data = malloc(sizeof(int) * 3);
        data[0] = ((int*)newDataPointer)[n];
        n++;
        data[1] = ((int*)newDataPointer)[n];
        n++;
        data[2] = ((int*)newDataPointer)[n];
        n++;
        AddItem(Layers, data, 3);
        item = item->next;
    }
    ti_Close(file);
    free(DataArray);
    return 0;
}
/* //From GUI:
    So that we dont forget what each number does
    gfx_PrintStringXY("I forgot  ", 10, 5); gfx_PrintInt(data[0], 1);
    gfx_PrintStringXY("World X  ", 10, 16);gfx_PrintInt(data[1], 1);
    gfx_PrintStringXY("World Y  ", 10, 27);gfx_PrintInt(data[2], 1);
    gfx_PrintStringXY("World Z  ", 10, 38);gfx_PrintInt(data[3], 1);
    gfx_PrintStringXY("Details  ", 10, 49);gfx_PrintInt(data[4], 1);
*/