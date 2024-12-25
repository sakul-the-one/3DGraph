#include "D3/3DGraphics.h"
#include "D3/3DRenderer.h"

/*void D3R_AddPoint(Vector3 pos) //Not implemented, bc not needed (waste of space)
{
    Vector3 * Backup = NULL;
    if(_DPDCount != 0) Backup = _DPD;
    _DPDCount++;
    _DPD = malloc(_DPDCount* sizeof(Vector3));

    if (_DPD == NULL) { //Handle faulty malloc
        _DPDCount--;
        if (Backup != NULL) 
            _DPD = Backup; // Restore the old array
        return;
    }

    if(_DPDCount != 0) 
    {
        for(int i = 0; i < _DPDCount-1; i++)
            _DPD[i] = Backup[i];
        free(Backup);
    }
    _DPD[_DPDCount-1] = pos;
}//*/
void D3R_PreMallocLine(int times) {
    DrawDataLine *Backup = _DLD;
    _DLDCount += times;
    _DLD = malloc(_DLDCount * sizeof(DrawDataLine));
    if (_DLD == NULL) {
        _DLDCount -= times;
        _DLD = Backup;
        gfx_PrintStringXY("Error Pre-Mallocing",10,10);
        return;
    }

    if (Backup) {
        for (int i = 0; i < _DLDCount - times; i++) {
            _DLD[i] = Backup[i];
        }
        free(Backup);
    }
}
void D3R_AddLine(Vector3 pos1, Vector3 pos2) 
{
    DrawDataLine temp;
    // Bigger pos always first, for easier sorting later on ;)
    if (pos1.z > pos2.z) {
        temp.pos1 = pos1;
        temp.pos2 = pos2;
    } else {
        temp.pos1 = pos2;
        temp.pos2 = pos1;
    }

    if (temp.pos1.z < _lowestLD) _lowestLD = temp.pos1.z;

    if (_DLDCountUsed < _DLDCount) {
        _DLD[_DLDCountUsed] = temp;
        _DLDCountUsed++;
        return;
    }

    DrawDataLine *Backup = _DLD;
    _DLDCount++;
    _DLDCountUsed++;

    _DLD = malloc(_DLDCount * sizeof(DrawDataLine));
    if (_DLD == NULL) {
        _DLDCount--;
        _DLDCountUsed--;
        _DLD = Backup;
        return;
    }

    for (int i = 0; i < _DLDCount - 1; i++) {
        _DLD[i] = Backup[i];
    }
    free(Backup);
    _DLD[_DLDCount - 1] = temp;
}

void D3R_Clear() {
    _DLDCount = 0;
    _DLDCountUsed = 0;
    free(_DLD);
    _DLD = NULL;
}

void swap(DrawDataLine* a, DrawDataLine* b) {
    DrawDataLine t = *a;
    *a = *b;
    *b = t;
}
//Reconstructed for _DLD
// Partition function
int partition(DrawDataLine* arr, int low, int high) {
    float pivot = arr[high].pos1.z;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].pos1.z < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Quick sort function for DrawDataLine
void quickSort(DrawDataLine* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void D3R_SortLines() 
{
    Vector3 WorldRotation = D3G_GetWorldRotation();
    //Rotate
    for (int i = 0; i < _DLDCount; i++) 
    {
        _DLD[i].pos1 = D3G_RotatePoint(_DLD[i].pos1, WorldRotation);
        _DLD[i].pos2 = D3G_RotatePoint(_DLD[i].pos2, WorldRotation);
    }
    //Sort
    quickSort(_DLD, _lowestLD, _DLDCount-1);
}

void D3R_Draw() 
{
    D3R_SortLines();
    //D3R_SortPoints(); //Not implemented, bc not needed (waste of space)
    for (int i = 0; i < _DLDCount; i++) 
    {
        D3G_DrawLineUnRotated(_DLD[i].pos1, _DLD[i].pos2);
    }
    D3R_Clear();
}