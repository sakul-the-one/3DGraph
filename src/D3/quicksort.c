#include <stdio.h>
#include "D3/3DRenderer.h"
// Partition function
int partition(int arr[], int low, int high) {
    
    // Choose the pivot
    int pivot = arr[high];
    
    // Index of smaller element and indicates 
    // the right position of pivot found so far
    int i = low - 1;

    // Traverse arr[low..high] and move all smaller
    // elements to the left side. Elements from low to 
    // i are smaller after every iteration
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    
    // Move pivot after smaller elements and
    // return its position
    swap(&arr[i + 1], &arr[high]);  
    return i + 1;
}

// The QuickSort function implementation
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        
        // pi is the partition return index of pivot
        int pi = partition(arr, low, high);

        // Recursion calls for smaller elements
        // and greater or equals elements
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void swap(float* a, float* b) {
    float t = *a;
    *a = *b;
    *b = t;
}
//Reconstructed for _DLD
// Partition function
int partition(DrawDataLine * arr[], float low, int high) {
    
    // Choose the pivot
    float pivot = arr[high]->pos1.z;
    
    // Index of smaller element and indicates 
    // the right position of pivot found so far
    int i = low - 1;

    // Traverse arr[low..high] and move all smaller
    // elements to the left side. Elements from low to 
    // i are smaller after every iteration
    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->pos1.z < pivot) {
            i++;
            swap(&arr[i]->pos1.z, &arr[j]->pos1.z);
        }
    }
    
    // Move pivot after smaller elements and
    // return its position
    swap(&arr[i + 1]->pos1.z, &arr[high]->pos1.z);  
    return i + 1;
}

// The QuickSort function implementation
void quickSort(DrawDataLine * arr[], float low, int high) {
    if (low < high) {
        
        // pi is the partition return index of pivot
        int pi = partition(arr, low, high);

        // Recursion calls for smaller elements
        // and greater or equals elements
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}