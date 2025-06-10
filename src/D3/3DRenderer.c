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
    DrawDataLine *Backup = _DLD;//Make Back-up of Pointer
    _DLDCount += times; //Max Number of free spaces
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
void D3R_AddLine(Vector3 pos1, Vector3 pos2, int8_t ccolor) 
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
    temp.color = ccolor;

    //if (temp.pos1.z < _lowestLD) _lowestLD = temp.pos1.z;
    //when Used spaces less then max number of spaces (when Premalloc was used); A backup is not needed
    if (_DLDCountUsed < _DLDCount) {
        _DLD[_DLDCountUsed] = temp; //Sets the next space (bc array starts at [0], number just pasted in)
        _DLDCountUsed++; //Increases used
        return; //Returns the entire fuction
    }
    //When not:
    DrawDataLine *Backup = _DLD; //Backup
    //Increases both space and used
    _DLDCount++;
    _DLDCountUsed++;
    _DLD = NULL;
    //Make new pointer
    _DLD = malloc(_DLDCount * sizeof(DrawDataLine));
    if (_DLD == NULL) { //If it fails
        _DLDCount--;
        _DLDCountUsed--;
        _DLD = Backup;
        gfx_PrintStringXY("Error Mallocing",10,20);
        return;
    }
    //Copies old Data from Backup
    for (int i = 0; i < _DLDCount - 1; i++) {
        _DLD[i] = Backup[i];
    }
    free(Backup); //Free the backup
    _DLD[_DLDCount - 1] = temp; // Puts temp data into needed position (-1, becausebc array starts at [0], but not our number)
}

void D3R_Clear() { //Clears everything used
    _DLDCount = 0;
    _DLDCountUsed = 0;
    //_lowestLD = 255.0f;
    if(_DLD == NULL) return;
    free(_DLD);
    _DLD = NULL;

    Rotated = false;
}

void swap(DrawDataLine* a, DrawDataLine* b) {
    DrawDataLine t = *a;
    *a = *b;
    *b = t;
}
//Reconstructed for _DLD
// Partition function
/* //Qucksort bad for calc :C; FUCK YOU QUICKSORT; INSERTIONSSORT IS MY HOMIE NOW!
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
}*/

void insertionSort(DrawDataLine arr[], int n) {
    for (int i = 1; i < n; i++) 
    {
        DrawDataLine key = arr[i]; // key is the whole DrawDataLine structure
        int j = i - 1;
        // Move elements of arr[0..i-1], that are greater than key.pos1.z,
        // to one position ahead of their current position
        while (j >= 0 && arr[j].pos1.z > key.pos1.z) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void D3R_SortLines(bool sortlines) 
{
    if(Rotated) return;
    Vector3 WorldRotation = D3G_GetWorldRotation();
    //Rotate
    for (int i = 0; i < _DLDCount; i++) 
    {
        D3G_RotatePoint(&_DLD[i].pos1, WorldRotation);
        D3G_RotatePoint(&_DLD[i].pos2, WorldRotation);
    }
    //Sort
    if(sortlines)
        insertionSort(_DLD, _DLDCount);
    Rotated = true;
    //quickSort(_DLD, _lowestLD, _DLDCount-1);
}
int8_t CurrentColour;
void D3R_Draw(bool sortlines) 
{    
    D3R_SortLines(sortlines);
    //Debug
    //printf("%d + %d ", _DLDCount, _DLDCountUsed);
    for (int i = 0; i < _DLDCount; i++) 
    {
        if (CurrentColour != _DLD[i].color)
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-W#pragma-messages"
            switch (_DLD[i].color)
            {
                case 0: CurrentColour = gfx_black; break;
                case 1: CurrentColour= gfx_blue; break;
                case 2: CurrentColour = gfx_red; break;
                case 3: CurrentColour = gfx_green; break;
                case 4: CurrentColour = gfx_yellow; break;
                case 5: CurrentColour = gfx_orange; break;
                case 6: CurrentColour = gfx_pink; break;
                default: CurrentColour = _DLD[i].color*20; break;
            }
#pragma GCC diagnostic pop
            gfx_SetColor(CurrentColour);
        }
        D3G_DrawLineUnRotated(_DLD[i].pos1, _DLD[i].pos2);
    }
}