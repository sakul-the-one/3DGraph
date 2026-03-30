#include "StaticData.h"
#include <fileioc.h>
#include <string.h>
#include "D3/3DRenderer.h"
#include "LinkedList.h"
#include "D3Handler.h"

#define HEADER_SIZE 2

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
        gfx_PrintStringXY("Error saving! RAM to full",0,0);
        return;
    }
    ((int*)DataArray)[0] = (int)mallocSize; //safe the size
    ((int*)DataArray)[1] = Lines->count;
    int i = 0;
    for (; i < 26; i++) //26 for every letter
    {
        Vector3 p;
        GetPoint(i, &p);
        ((float*)DataArray)[i * 3 + HEADER_SIZE + 0] = p.x; //+2 for header vars.
        ((float*)DataArray)[i * 3 + HEADER_SIZE + 1] = p.y; //+3 for header vars and first X variable
        ((float*)DataArray)[i * 3 + HEADER_SIZE + 2] = p.z; // --""--
    }
    i += 26 * 2 + HEADER_SIZE; ///*26 * 2 + 2 = 53*/ //i is already 26, so add just twice of 26 and the Headersize. I hope the compiler optimzes it
    //Save Lines:
    int n = 0;
    LinkedItem * item = Lines->first;
    while (n < Lines->count)
    {
        int * data = item->Data;
        for (int j = 0; j < item->DataLenght; j++) 
        {
            ((int*)DataArray)[i] = data[j];
            i++;
            item = item->next;
        }
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
            ((int*)DataArray)[i] = data[j];
            i++;
            item = item->next;
        }
        n++;
    }
    //Write&Close
    ti_Write(DataArray, mallocSize, 1, file); //yes. It should not be made like this. But I made it like this. This is a Datadump.
    ti_Close(file);
    free(DataArray);
}
void LoadVectorData() 
{
    uint8_t file = ti_Open("D3SV","r"); //D3GS: Save Vectors
    if (file == 0) return; //If failed, ignore it
    int fileSize = 0;
    ti_Read(&fileSize, sizeof(int), 1, file);
    uint8_t * DataArray = malloc(fileSize);
    if (DataArray == 0) { //Not enough Ram, ignore it again!
        ti_Delete(file);
        ti_Close(file);
        return;
    }
    //Actually Parsing the Data now :)
}
/* //From GUI:
    So that we dont forget what each number does
    gfx_PrintStringXY("I forgot  ", 10, 5); gfx_PrintInt(data[0], 1);
    gfx_PrintStringXY("World X  ", 10, 16);gfx_PrintInt(data[1], 1);
    gfx_PrintStringXY("World Y  ", 10, 27);gfx_PrintInt(data[2], 1);
    gfx_PrintStringXY("World Z  ", 10, 38);gfx_PrintInt(data[3], 1);
    gfx_PrintStringXY("Details  ", 10, 49);gfx_PrintInt(data[4], 1);
*/