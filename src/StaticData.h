#ifndef STDT_H
#define STDT_H

#include <stdint.h>

float * DataArray;

void InitData();
void SetDataArray(float * NewData);
void SetDataValue(float NewData, uint8_t pos);
float * GetDataArray();
void CloseData();
//Saving Vectors:
void SaveVectorData();
int LoadVectorData();
#endif