#ifndef STDT_H
#define STDT_H

float * DataArray;

void InitData();
void SetDataArray(float * NewData);
void SetDataValue(float NewData, uint8_t pos);
float * GetDataArray();
void CloseData();
//Saving Vectors:
void SaveVectorData();
void LoadVectorData();
#endif