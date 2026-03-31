#ifndef LISTS_H
#define LISTS_H
#include <fileioc.h>
//C compiler?? Wtf is this shit?? Why Do I need to shit this to make it work?
typedef struct LinkedItem;
typedef struct
{
    void * Data;
    int DataLenght;
    struct LinkedItem * next;
} LinkedItem;

typedef struct
{
    LinkedItem * first;
    LinkedItem * last;
    int count;
} LinkedList;

LinkedList * CreateList();
void AddItem(LinkedList * List, void * Data, int DataLenght);
void RemoveItem(LinkedList * List, uint8_t pos);
void DestroyList(LinkedList * List);
#endif